#!/usr/bin/env python3
"""Check the landing tools (apply_commit.py, header_merge.py) on small repos.

    python3 tools/tests/test_landing.py     (or: make test)

Each case builds a throwaway git repo under a temporary directory with a
base commit, a mainline commit and a branch commit, then replays the branch
commit onto mainline with `apply_commit.py --no-commit` and checks the
files. Nothing in this checkout is touched, and no ROM or build is needed.
"""

import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent

TYPES_H = """\
#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#endif
"""


class Repo:
    """A throwaway git repo with the files apply_commit.py expects."""

    def __init__(self, path: Path):
        self.path = path
        self.git("init", "-q", "-b", "mainline")
        self.git("config", "user.name", "test")
        self.git("config", "user.email", "test@example.com")
        self.git("config", "core.hooksPath", str(path / ".no-hooks"))
        self.write("include/types.h", TYPES_H)
        self.write("docs/README.md", "test\n")
        self.write("src/main.c", "int main(void) { return 0; }\n")
        self.write("config/AWRE/arm9/symbols.txt", "Entry kind:function(arm,size=0x4) addr:0x02000800\n")

    def git(self, *args: str) -> str:
        env = {**os.environ, "GIT_CONFIG_GLOBAL": os.devnull, "GIT_CONFIG_NOSYSTEM": "1"}
        return subprocess.run(["git", *args], cwd=self.path, capture_output=True, text=True,
                              check=True, env=env).stdout.strip()

    def write(self, rel: str, text: str) -> None:
        p = self.path / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)

    def read(self, rel: str) -> str:
        return (self.path / rel).read_text()

    def commit(self, files: dict[str, str], msg: str) -> str:
        for rel, text in files.items():
            self.write(rel, text)
        self.git("add", "-A")
        self.git("commit", "-q", "-m", msg)
        return self.git("rev-parse", "HEAD")

    def replay(self, base: dict[str, str], ours: dict[str, str], theirs: dict[str, str]):
        """Commit base, then theirs on a branch and ours on mainline; replay
        the branch commit onto mainline. Returns the finished process."""
        self.commit(base, "base")
        self.git("switch", "-q", "-c", "agent")
        commit = self.commit(theirs, "agent change")
        self.git("switch", "-q", "mainline")
        self.commit(ours, "mainline change")
        return subprocess.run([sys.executable, str(TOOLS / "apply_commit.py"), commit, "--no-commit"],
                              cwd=self.path, capture_output=True, text=True)


def struct(name: str, body: str) -> str:
    guard = name.upper() + "_H"
    return (f'#ifndef {guard}\n#define {guard}\n\n#include "types.h"\n\n'
            f"typedef struct {name} {{\n{body}}} {name};\n\n#endif\n")


class RepoCase(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.repo = Repo(Path(self.tmp.name))

    def tearDown(self):
        self.tmp.cleanup()


class FieldRenameTest(RepoCase):
    def test_field_rename_scoped_to_struct(self):
        """Issue 0025: a header merge renamed Foo.mapWidth to mainline's name,
        and every `->mapWidth` in the commit's C changed, Bar's too."""
        bar = struct("Bar", "    u16 mapWidth;\n    u16 mapHeight;\n")
        foo = "    u8 pad_00[0x44];\n    u16 {a};\n    u8 pad_46[0x2];\n    u32 {b};\n"
        src = ('#include "foo.h"\n#include "bar.h"\n\n'
               "int f(Foo *foo, Bar *bar) {\n"
               "    return foo->mapWidth + foo->depth + bar->mapWidth;\n}\n")
        r = self.repo.replay(
            base={"include/foo.h": struct("Foo", foo.format(a="unk_44", b="unk_48")), "include/bar.h": bar},
            ours={"include/foo.h": struct("Foo", foo.format(a="width", b="h"))},
            theirs={"include/foo.h": struct("Foo", foo.format(a="mapWidth", b="depth")), "src/f.c": src})
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("Foo.mapWidth is width on mainline, but Bar also has a field mapWidth", r.stderr)
        self.assertIn("src/f.c:5", r.stderr)
        text = self.repo.read("src/f.c")
        self.assertIn("bar->mapWidth", text)   # not renamed: Bar has its own mapWidth
        self.assertIn("foo->mapWidth", text)   # left for the agent, named in the report
        self.assertIn("foo->h", text)          # depth is only Foo's: renamed

    def test_field_rename_without_clash(self):
        foo = "    u8 pad_00[0x44];\n    u16 {a};\n    u8 pad_46[0x2];\n"
        src = '#include "foo.h"\n\nint f(Foo *foo) { return foo->mapWidth; }\n'
        r = self.repo.replay(
            base={"include/foo.h": struct("Foo", foo.format(a="unk_44"))},
            ours={"include/foo.h": struct("Foo", foo.format(a="width"))},
            theirs={"include/foo.h": struct("Foo", foo.format(a="mapWidth")), "src/f.c": src})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("foo->width", self.repo.read("src/f.c"))


def header(body: str) -> str:
    return f'#ifndef T_H\n#define T_H\n\n#include "types.h"\n\n{body}\n#endif\n'


ENUM = "enum TerrainId {{\n    TERRAIN_NONE,\n    TERRAIN_PLAIN,\n{a}    TERRAIN_LAB = 20,\n    TERRAIN_TOWER = 22{b}\n}};\n"


class HeaderUnionTest(RepoCase):
    def merged_values(self) -> dict[str, str]:
        import header_merge  # tools/ is on sys.path below
        return header_merge.Types([self.repo.read("include/t.h")]).defines

    def test_enum_constants_union(self):
        """Issue 0025: TERRAIN_* constants were dropped when both sides added
        some next to TERRAIN_LAB."""
        r = self.repo.replay(
            base={"include/t.h": header(ENUM.format(a="", b=""))},
            ours={"include/t.h": header(ENUM.format(a="    TERRAIN_CITY = 6,\n", b=""))},
            theirs={"include/t.h": header(ENUM.format(a="    TERRAIN_SILO = 17,  /* launches once */\n",
                                                      b=",\n    TERRAIN_VOLCANO = 28"))})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        v = self.merged_values()
        for name, value in [("TERRAIN_PLAIN", 1), ("TERRAIN_CITY", 6), ("TERRAIN_SILO", 17),
                            ("TERRAIN_LAB", 20), ("TERRAIN_TOWER", 22), ("TERRAIN_VOLCANO", 28)]:
            self.assertEqual(v.get(name), str(value), name)
        text = self.repo.read("include/t.h")
        self.assertIn("TERRAIN_SILO = 17, /* launches once */", text)
        self.assertIn("TERRAIN_VOLCANO = 28\n};", text)

    def test_enum_constant_clash(self):
        r = self.repo.replay(
            base={"include/t.h": header(ENUM.format(a="", b=""))},
            ours={"include/t.h": header(ENUM.format(a="    TERRAIN_SILO = 17,\n", b=""))},
            theirs={"include/t.h": header(ENUM.format(a="    TERRAIN_SILO = 18,\n", b=""))})
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("TERRAIN_SILO is 17 on mainline and 18 in the commit", r.stderr)

    def test_defines_union(self):
        r = self.repo.replay(
            base={"include/t.h": header("#define FLAG_A 1\n")},
            ours={"include/t.h": header("#define FLAG_A 1\n#define FLAG_B 2\n")},
            theirs={"include/t.h": header("#define FLAG_A 1\n#define FLAG_C 4\n#define FLAG_B  2\n")})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        text = self.repo.read("include/t.h")
        for name in ("FLAG_A 1", "FLAG_B 2", "FLAG_C 4"):
            self.assertEqual(text.count(f"#define {name}"), 1, name)

    def test_define_clash(self):
        r = self.repo.replay(
            base={"include/t.h": header("#define FLAG_A 1\n")},
            ours={"include/t.h": header("#define FLAG_A 1\n#define FLAG_B 2\n")},
            theirs={"include/t.h": header("#define FLAG_A 1\n#define FLAG_B 8\n")})
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("#define FLAG_B is `2` on mainline and `8` in the commit", r.stderr)


sys.path.insert(0, str(TOOLS))

if __name__ == "__main__":
    unittest.main()
