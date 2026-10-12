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
import time
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

    def commit(self, files: dict[str, str | None], msg: str) -> str:
        """Write files and commit them; a file whose text is None is deleted."""
        for rel, text in files.items():
            if text is None:
                (self.path / rel).unlink()
            else:
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


class AnonymousUnionTest(RepoCase):
    """Both sides lay an anonymous union over the same field (b12/o3s2:
    Ov003State.unk_1d6, Unk_ov003_0235dd38.unk_4c)."""
    BASE = "    u8 a;\n    u8 b;\n    u16 unk_2;\n    u8 c;\n"
    VIEW = ("    u8 a;\n    u8 b;\n    union {{\n        u16 unk_2;\n        struct {{\n"
            "{inner}        }};\n    }};\n    u8 c;\n{tail}")

    def test_both_sides_add_members(self):
        r = self.repo.replay(
            base={"include/foo.h": struct("Foo", self.BASE)},
            ours={"include/foo.h": struct("Foo", self.VIEW.format(
                inner="            u8 pad_2;\n            u8 unk_3;  /* waits while nonzero */\n", tail=""))},
            theirs={"include/foo.h": struct("Foo", self.VIEW.format(
                inner="            u8 flags;\n            u8 pad_3;\n", tail="    u8 d;\n"))})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        text = self.repo.read("include/foo.h")
        for name in ("u8 flags;", "u8 unk_3;  /* waits while nonzero */", "u16 unk_2;", "u8 d;"):
            self.assertEqual(text.count(name), 1, name + "\n" + text)

    def test_bit_field_named_pad(self):
        """A bit-field named pad_* followed by another one in the same byte
        crashed the merge (TypeError at `last.name +=`)."""
        r = self.repo.replay(
            base={"include/foo.h": struct("Foo", self.BASE)},
            ours={"include/foo.h": struct("Foo", self.VIEW.format(
                inner="            u8 pad_2;\n            u8 unk_3;\n", tail=""))},
            theirs={"include/foo.h": struct("Foo", self.VIEW.format(
                inner="            u8 pad_2_0 : 4;\n            u8 unk_2_4 : 4;\n            u8 unk_3;\n",
                tail=""))})
        self.assertNotIn("Traceback", r.stderr)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("u8 unk_2_4 : 4;", self.repo.read("include/foo.h"))


class EmbeddedStructSizeTest(RepoCase):
    """b12/o4s2: mainline grew a struct that another one embeds, and the
    commit's fields after it were placed with mainline's size, 4 bytes late
    (Unk_ov004_022bf158: false renames, then a field clash)."""
    INNER = "typedef struct Inner {{\n{0}    u16 unk_0c;\n    u16 unk_0e;\n{1}}} Inner;\n\n"
    OUTER = "typedef struct Outer {{\n    u8 pad_00[0x8];\n    Inner inner;\n{0}    u32 {1};\n}} Outer;\n"

    def version(self, inner_head: str, inner_tail: str, pad: str, name: str) -> str:
        return header(self.INNER.format(inner_head, inner_tail) + self.OUTER.format(pad, name))

    def test_commit_fields_after_a_grown_struct(self):
        r = self.repo.replay(
            base={"include/t.h": self.version("    u8 pad_00[0xc];\n", "", "    u8 pad_18[0x40 - 0x18];\n", "unk_40")},
            ours={"include/t.h": self.version("    u8 pad_00[0xc];\n", "    u16 unk_10;\n",
                                              "    u8 pad_1c[0x40 - 0x1c];\n", "unk_40")},
            theirs={"include/t.h": self.version("    u32 unk_00;\n    u8 pad_04[0x8];\n", "",
                                                "    u8 pad_18[0x40 - 0x18];\n", "count")})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertNotIn("renamed", r.stdout)
        text = self.repo.read("include/t.h")
        for line in ("u32 unk_00;", "u16 unk_10;", "u32 count;"):
            self.assertEqual(text.count(line), 1, line + "\n" + text)
        self.assertNotIn("unk_40", text)


class RemovedFieldTest(RepoCase):
    """b23/mb8 and b23/mb6: mainline removed DlPlayHostMsgHead.pad_06 and
    the struct shrank to 6 bytes; a commit made on the older header put it
    back, and the ROM stopped matching."""
    HEAD = "    u8 type;\n    u8 pad_01;\n    u16 fileId;\n"

    def test_mainline_removal_stays(self):
        r = self.repo.replay(
            base={"include/mb.h": struct("Hdr", self.HEAD + "    u16 unk_04;\n    u8 pad_06[2];\n")},
            ours={"include/mb.h": struct("Hdr", self.HEAD + "    u16 unk_04;\n")},
            theirs={"include/mb.h": struct("Hdr", self.HEAD + "    u16 block;\n    u8 pad_06[2];\n")})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("merged include/mb.h", r.stdout)
        text = self.repo.read("include/mb.h")
        self.assertIn("u16 block;", text)
        self.assertNotIn("pad_06", text)

    def test_commit_removal_stays(self):
        r = self.repo.replay(
            base={"include/mb.h": struct("Hdr", self.HEAD + "    u16 unk_04;\n    u16 unk_06;\n")},
            ours={"include/mb.h": struct("Hdr", self.HEAD + "    u16 block;\n    u16 unk_06;\n")},
            theirs={"include/mb.h": struct("Hdr", self.HEAD + "    u16 unk_04;\n")})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        text = self.repo.read("include/mb.h")
        self.assertIn("u16 block;", text)
        self.assertNotIn("unk_06", text)

    def test_removal_against_change(self):
        r = self.repo.replay(
            base={"include/mb.h": struct("Hdr", self.HEAD + "    u16 unk_04;\n    u16 unk_06;\n")},
            ours={"include/mb.h": struct("Hdr", self.HEAD + "    u16 block;\n")},
            theirs={"include/mb.h": struct("Hdr", self.HEAD + "    u16 unk_04;\n    u16 count;\n")})
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("Hdr.count (0x6, 2 bytes): mainline removed the base's unk_06", r.stderr)

    def test_union_member_removal(self):
        view = "    union {{\n        u32 word;\n{0}    }};\n    u8 {1};\n"
        r = self.repo.replay(
            base={"include/mb.h": struct("Hdr", view.format("        u16 half;\n", "unk_4"))},
            ours={"include/mb.h": struct("Hdr", view.format("", "kind"))},
            theirs={"include/mb.h": struct("Hdr", view.format("        u8 half;\n", "unk_4"))})
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("Hdr.half: mainline removed it from the union", r.stderr)


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


RELOCS = "config/AWRE/arm9/relocs.txt"
SYMBOLS = "config/AWRE/arm9/symbols.txt"


def reloc(at: int, to: int) -> str:
    return f"from:0x{at:08x} kind:arm_call to:0x{to:08x} module:main\n"


def symbol(name: str, at: int) -> str:
    return f"{name} kind:function(arm,size=0x4) addr:0x{at:08x}\n"


class ConfigReplayTest(RepoCase):
    def test_relocs_removed_and_changed(self):
        """Issue 0025: lines a commit removed from relocs.txt were not replayed."""
        base = reloc(0x2000800, 0x2000a00) + reloc(0x2000804, 0x2000a04) + reloc(0x2000808, 0x2000a08)
        r = self.repo.replay(
            base={RELOCS: base},
            ours={RELOCS: base + reloc(0x200080c, 0x2000a0c)},
            theirs={RELOCS: reloc(0x2000800, 0x2000a00) + reloc(0x2000808, 0x2000b08)})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertEqual(self.repo.read(RELOCS),
                         reloc(0x2000800, 0x2000a00) + reloc(0x2000808, 0x2000b08) + reloc(0x200080c, 0x2000a0c))

    def test_relocs_clash(self):
        base = reloc(0x2000800, 0x2000a00)
        r = self.repo.replay(
            base={RELOCS: base},
            ours={RELOCS: reloc(0x2000800, 0x2000c00)},
            theirs={RELOCS: reloc(0x2000800, 0x2000b00)})
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("this branch has `from:0x02000800", r.stderr)

    def test_symbols_removed_renamed_added(self):
        base = symbol("Entry", 0x2000800) + symbol("func_02000900", 0x2000900) + symbol("func_02000a00", 0x2000a00)
        r = self.repo.replay(
            base={SYMBOLS: base},
            ours={SYMBOLS: base + symbol("func_02000b00", 0x2000b00)},
            theirs={SYMBOLS: symbol("Entry", 0x2000800) + symbol("Unit_Move", 0x2000a00)
                    + symbol("func_02000c00", 0x2000c00)})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertEqual(self.repo.read(SYMBOLS),
                         symbol("Entry", 0x2000800) + symbol("Unit_Move", 0x2000a00)
                         + symbol("func_02000b00", 0x2000b00) + symbol("func_02000c00", 0x2000c00))


    def test_new_config_directory(self):
        """A commit that adds config files mainline does not have (ARM7's
        config/AWRE/arm7/) crashed the replay of delinks.txt."""
        delinks = "    .text       start:0x02380000 end:0x02380158 kind:code align:32\n\n"
        files = {"config/AWRE/arm7/delinks.txt": delinks,
                 "config/AWRE/arm7/symbols.txt": symbol("func_arm7_02380000", 0x2380000),
                 "config/AWRE/arm7/relocs.txt": reloc(0x2380000, 0x2380010)}
        r = self.repo.replay(base={}, ours={"docs/README.md": "changed\n"}, theirs=files)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        for path, text in files.items():
            self.assertEqual(self.repo.read(path), text)


class MovedFileTest(RepoCase):
    """Issue 0046: a replayed move left the old file when mainline had
    renamed names in it, and an edit to a file mainline had moved wrote the
    old path again."""

    OLD, NEW = "src/func_02000900.c", "src/map/map_draw.c"
    BODY = "void Map_Hook(void);\nvoid {name}(void) {{ {hook}(); x = {n}; }}\n"

    def body(self, name="func_02000900", hook="func_02000a00", n=1):
        return self.BODY.format(name=name, hook=hook, n=n)

    def test_move_after_mainline_rename(self):
        r = self.repo.replay(
            base={self.OLD: self.body()},
            ours={self.OLD: self.body(hook="Map_Hook")},
            theirs={self.OLD: None, self.NEW: self.body(name="Map_Draw")})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertFalse((self.repo.path / self.OLD).exists())
        self.assertTrue((self.repo.path / self.NEW).exists())

    def test_move_after_mainline_edit(self):
        r = self.repo.replay(
            base={self.OLD: self.body()},
            ours={self.OLD: self.body(n=2)},
            theirs={self.OLD: None, self.NEW: self.body(name="Map_Draw")})
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("the commit deletes it", r.stderr)
        self.assertTrue((self.repo.path / self.OLD).exists())

    def test_rename_in_file_mainline_moved(self):
        r = self.repo.replay(
            base={SYMBOLS: symbol("func_02000900", 0x2000900), self.OLD: self.body()},
            ours={self.OLD: None, self.NEW: self.body()},
            theirs={SYMBOLS: symbol("Map_Draw", 0x2000900), self.OLD: self.body(name="Map_Draw")})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertFalse((self.repo.path / self.OLD).exists())
        self.assertEqual(self.repo.read(self.NEW), self.body(name="Map_Draw"))

    def test_edit_in_file_mainline_moved(self):
        r = self.repo.replay(
            base={self.OLD: self.body()},
            ours={self.OLD: None, self.NEW: self.body(hook="Map_Hook")},
            theirs={self.OLD: self.body(n=3)})
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn(f"perhaps to {self.NEW}", r.stderr)
        self.assertFalse((self.repo.path / self.OLD).exists())


class ChainedRenameTest(RepoCase):
    """Issue 0030: a commit renamed A Count -> CountAny and gave Count to C
    (func_02000a00); the replay renamed in sequence and C's callers ended
    up calling CountAny."""

    def check(self, symbols_in_order):
        a, c = (0x2000900, "Count", "CountAny"), (0x2000a00, "func_02000a00", "Count")
        rows = sorted([a, c], key=lambda s: s[0], reverse=not symbols_in_order)
        caller = "int Count(void);\nint {c}(void);\nint f(void) {{ return {a}() + {c}(); }}\n"
        r = self.repo.replay(
            base={SYMBOLS: "".join(symbol(old, at) for at, old, _ in rows),
                  "src/a.c": "int Count(void) { return 1; }\n",
                  "src/caller.c": caller.format(a="Count", c="func_02000a00"),
                  "src/other.c": caller.format(a="Count", c="func_02000a00")},
            ours={"src/main.c": "int main(void) { return 1; }\n"},
            theirs={SYMBOLS: "".join(symbol(new, at) for at, _, new in rows),
                    "src/a.c": "int CountAny(void) { return 1; }\n",
                    "src/c.c": "int Count(void) { return 2; }\n",
                    "src/caller.c": caller.format(a="CountAny", c="Count").replace("int Count(void);\n", "int CountAny(void);\n", 1)})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertEqual(self.repo.read(SYMBOLS), "".join(symbol(new, at) for at, _, new in rows))
        self.assertIn("int Count(void) { return 2; }", self.repo.read("src/c.c"))
        want = "return CountAny() + Count();"
        self.assertIn(want, self.repo.read("src/caller.c"))   # the commit's file, as committed
        self.assertIn(want, self.repo.read("src/other.c"))    # mainline's file, renamed here

    def test_symbols_by_address(self):
        self.check(symbols_in_order=True)

    def test_symbols_out_of_order(self):
        self.check(symbols_in_order=False)


class LandSelfBusyTest(RepoCase):
    """Issue 0025: a land_self.py run left in the background by a tool
    timeout overlapped a second run in the same worktree."""

    def setUp(self):
        super().setUp()
        self.repo.commit({}, "base")
        self.wt = Path(self.tmp.name) / "wt"
        self.repo.git("worktree", "add", "-q", "-b", "agent", str(self.wt))
        self.gitdir = Path(self.repo.git("-C", str(self.wt), "rev-parse", "--path-format=absolute", "--git-dir"))

    def run_tool(self, *args: str):
        env = {k: v for k, v in os.environ.items() if k != "LAND_SELF_LOCK_HELD"}
        return subprocess.run([sys.executable, *args], cwd=self.wt, capture_output=True, text=True, env=env)

    def test_refuses_open_replay(self):
        (self.gitdir / "rebase-onto-mainline.json").write_text("{}")
        r = self.run_tool(str(TOOLS / "land_self.py"))
        self.assertEqual(r.returncode, 4, r.stdout + r.stderr)
        self.assertIn("a replay is open in this worktree", r.stderr)

    def test_refuses_second_run(self):
        import fcntl
        with open(self.gitdir / "land-self.lock", "a+") as lock:
            fcntl.flock(lock, fcntl.LOCK_EX)
            lock.write("pid 1, land_self.py\n")
            lock.flush()
            r = self.run_tool(str(TOOLS / "land_self.py"))
            self.assertEqual(r.returncode, 4, r.stdout + r.stderr)
            self.assertIn("another land_self.py or rebase_onto_mainline.py runs in this worktree (pid 1", r.stderr)
            r = self.run_tool(str(TOOLS / "rebase_onto_mainline.py"), "--continue")
            self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
            self.assertIn("another land_self.py or rebase_onto_mainline.py runs", r.stderr)
        # Free again: the run goes past the lock (and stops later, at "nothing to land").
        r = self.run_tool(str(TOOLS / "land_self.py"))
        self.assertNotEqual(r.returncode, 4, r.stdout + r.stderr)


class ReviewStampTest(RepoCase):
    """2026-10-09: two agent groups landed commits that skipped the tenets
    review; land_self.py now refuses commits without a review stamp."""

    def setUp(self):
        super().setUp()
        self.repo.commit({"include/t.h": header("#define FLAG_A 1\n"),
                          "src/a.c": "int a(void)\n{\n    return 1;\n}\n\nint b(void)\n{\n    return 2;\n}\n"},
                         "base")
        self.wt = Path(self.tmp.name) / "wt"
        self.repo.git("worktree", "add", "-q", "-b", "agent", str(self.wt))

    def tool(self, name: str, *args: str):
        env = {k: v for k, v in os.environ.items() if k != "LAND_SELF_LOCK_HELD"}
        return subprocess.run([sys.executable, str(TOOLS / name), *args], cwd=self.wt,
                              capture_output=True, text=True, env=env)

    def commit(self, files: dict[str, str], msg: str) -> str:
        for rel, text in files.items():
            p = self.wt / rel
            p.parent.mkdir(parents=True, exist_ok=True)
            p.write_text(text)
        self.repo.git("-C", str(self.wt), "add", "-A")
        self.repo.git("-C", str(self.wt), "commit", "-q", "-m", msg)
        return self.repo.git("-C", str(self.wt), "rev-parse", "HEAD")

    def missing(self) -> list[str]:
        r = self.tool("review_stamp.py", "--missing")
        return [line.split()[0] for line in r.stdout.splitlines()]

    def stamp(self, *revs: str, findings: str | None = None):
        """Stamp with a findings line for every changed file under src/ and include/."""
        if findings is None:
            files = self.repo.git("-C", str(self.wt), "diff", "--cached", "--name-only") if not revs else \
                "\n".join(self.repo.git("-C", str(self.wt), "diff-tree", "--root", "-r", "--name-only",
                                        "--no-commit-id", rev) for rev in revs)
            findings = "".join(f"{f}: names, types, casts and comments checked; nothing to fix\n"
                               for f in files.split() if f.startswith(("src/", "include/")))
        path = Path(self.tmp.name) / "findings.txt"
        path.write_text(findings)
        return self.tool("review_stamp.py", "--findings", str(path), *revs)

    def test_land_refuses_unreviewed(self):
        c = self.commit({"src/f.c": "int f(void) { return 0; }\n"}, "Decompile f (main 0x02000900)")
        r = self.tool("land_self.py")
        self.assertEqual(r.returncode, 5, r.stdout + r.stderr)
        self.assertIn(c[:7], r.stderr)
        self.assertIn(f"python3 tools/review_stamp.py --findings FILE {c[:10]}", r.stderr)
        self.assertIn("tenets-review/SKILL.md", r.stderr)
        self.assertEqual(self.stamp(c).returncode, 0)
        r = self.tool("land_self.py")
        self.assertNotEqual(r.returncode, 5, r.stdout + r.stderr)  # goes on (no Makefile here)
        # The log shows what the review found.
        self.assertIn("review findings for", r.stdout)
        self.assertIn("src/f.c: names, types, casts and comments checked", r.stdout)

    def test_stamp_needs_findings(self):
        """2026-10-11: b19/s07 stamped 15 commits without reviewing them. A
        stamp now needs a findings line for each changed file."""
        c = self.commit({"src/f.c": "int f(void) { return 0; }\n",
                         "include/t.h": header("#define FLAG_A 1\n#define FLAG_F 2\n")}, "Decompile f")
        r = self.tool("review_stamp.py", c)
        self.assertEqual(r.returncode, 2, r.stdout + r.stderr)
        self.assertIn("--findings FILE is required", r.stderr)
        r = self.stamp(c, findings="src/f.c: checked names, types and comments, all fine\ninclude/t.h: ok\n")
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("no line `include/t.h: <what was checked and found>`", r.stderr)
        self.assertEqual(self.missing(), [c[:7]])
        r = self.stamp(c)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertEqual(self.missing(), [])
        self.assertIn("include/t.h: names", self.tool("review_stamp.py", "--show", c).stdout)

    def test_stamp_runs_style_check(self):
        word = next(w.rstrip("*") for w in (TOOLS.parent / "docs/style/banned.txt").read_text().splitlines()
                    if not w.startswith("#") and w.rstrip("*").isalpha())
        c = self.commit({"src/f.c": f"/* {word} */\nint f(void) {{ return 0; }}\n"}, "Decompile f")
        r = self.stamp(c)
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        self.assertIn("src/f.c:1: error: banned word", r.stdout)
        self.assertEqual(self.missing(), [c[:7]])

    def test_stamp_without_findings_is_missing(self):
        """Stamps written before findings were required count as missing."""
        c = self.commit({"src/f.c": "int f(void) { return 0; }\n"}, "Decompile f")
        self.assertEqual(self.stamp(c).returncode, 0)
        [old] = (self.repo.path / ".git/tenets-review").iterdir()
        old.write_text("2026-10-10T00:00:00Z staged change\n")
        self.assertEqual(self.missing(), [c[:7]])

    def test_stamp_staged_change(self):
        (self.wt / "src/f.c").write_text("int f(void) { return 0; }\n")
        self.repo.git("-C", str(self.wt), "add", "-A")
        r = self.stamp()
        self.assertIn("stamped the staged change", r.stdout)
        self.repo.git("-C", str(self.wt), "commit", "-q", "-m", "Decompile f")
        self.assertEqual(self.missing(), [])
        # PROGRESS.md amended into the commit does not change its stamp.
        (self.wt / "PROGRESS.md").write_text("progress\n")
        self.repo.git("-C", str(self.wt), "add", "-A")
        self.repo.git("-C", str(self.wt), "commit", "-q", "--amend", "--no-edit")
        self.assertEqual(self.missing(), [])
        # An edit after the stamp needs a new review.
        self.commit({"src/f.c": "int f(void) { return 1; }\n"}, "Fix f")
        self.assertEqual(len(self.missing()), 1)

    def test_exemptions(self):
        self.commit({"docs/notes.md": "notes\n"}, "Add notes")
        self.commit({"src/twin.c": "int twin(void) { return 3; }\n"}, "Propagate f to 1 near twin")
        self.commit({"src/wip.c": "int wip(void) { return 4; }\n"}, "WIP wip (95%)")
        self.assertEqual(self.missing(), [])
        wip = self.commit({"src/wip.c": "int wip(void) { return 5; }\n",
                           "include/t.h": header("#define FLAG_A 1\n#define FLAG_B 2\n")}, "WIP wip (97%)")
        prop = self.commit({"src/a.c": "int a(void)\n{\n    return 7;\n}\n"}, "Propagate a to 1 near twin")
        self.assertEqual(self.missing(), [wip[:7], prop[:7]])

    def test_stamp_survives_replay(self):
        c = self.commit({"src/a.c": "int a(void)\n{\n    return 1;\n}\n\nint b(void)\n{\n    return 3;\n}\n"},
                        "Decompile b")
        self.assertEqual(self.stamp(c).returncode, 0)
        # Mainline changes a line next to the commit's hunk, so the replayed
        # diff has other context lines and another patch ID.
        self.repo.commit({"src/a.c": "int a(void)\n{\n    return 1;\n}\n\nint b(void) /* b */\n{\n    return 2;\n}\n"},
                         "mainline change")
        r = self.tool("rebase_onto_mainline.py")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertNotEqual(self.repo.git("-C", str(self.wt), "rev-parse", "HEAD"), c)
        self.assertEqual(self.missing(), [])
        # Two stamps: the patch ID changed and the replay carried the stamp over.
        self.assertEqual(len(list((self.repo.path / ".git/tenets-review").iterdir())), 2)

    def test_hand_fix_needs_new_stamp(self):
        """2026-10-11: agents changed C by hand while fixing replay conflicts
        (b21/s27, b21/m17), and the replay stamped the result. A commit
        committed by --continue now needs a new review."""
        c = self.commit({"src/a.c": "int a(void)\n{\n    return 1;\n}\n\nint b(void)\n{\n    return 3;\n}\n"},
                        "Decompile b")
        self.assertEqual(self.stamp(c).returncode, 0)
        self.repo.commit({"src/a.c": "int a(void)\n{\n    return 1;\n}\n\nint b(void)\n{\n    return 4;\n}\n"},
                         "mainline change")
        r = self.tool("rebase_onto_mainline.py")
        self.assertEqual(r.returncode, 1, r.stdout + r.stderr)
        (self.wt / "src/a.c").write_text("int a(void)\n{\n    return 1;\n}\n\nint b(void)\n{\n    return 5;\n}\n")
        r = self.tool("rebase_onto_mainline.py", "--continue")
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("fixed by hand, so it needs a new tenets review", r.stdout)
        new = self.repo.git("-C", str(self.wt), "rev-parse", "HEAD")
        self.assertEqual(self.missing(), [new[:7]])
        r = self.tool("land_self.py")
        self.assertEqual(r.returncode, 5, r.stdout + r.stderr)
        self.assertEqual(self.stamp(new).returncode, 0)
        self.assertEqual(self.missing(), [])


class LandSelfReexecTest(RepoCase):
    """2026-10-09: a land_self.py run that started before the review check
    reached mainline kept its old code after the replay and landed commits
    without a review. A replay that changes the tool now restarts the run
    from the new copy."""

    TOOL_FILES = ("land_self.py", "review_stamp.py", "rebase_onto_mainline.py",
                  "apply_commit.py", "header_merge.py")

    def setUp(self):
        super().setUp()
        self.repo.commit({f"tools/{t}": (TOOLS / t).read_text() for t in self.TOOL_FILES}, "base")
        self.wt = Path(self.tmp.name) / "wt"
        self.repo.git("worktree", "add", "-q", "-b", "agent", str(self.wt))

    def land(self):
        env = {k: v for k, v in os.environ.items() if k != "LAND_SELF_LOCK_HELD"}
        return subprocess.run([sys.executable, "tools/land_self.py"], cwd=self.wt,
                              capture_output=True, text=True, env=env)

    def agent_commit(self):
        (self.wt / "notes.txt").write_text("notes\n")
        self.repo.git("-C", str(self.wt), "add", "-A")
        self.repo.git("-C", str(self.wt), "commit", "-q", "-m", "Add notes")

    def test_restarts_with_new_copy(self):
        self.agent_commit()
        land = (TOOLS / "land_self.py").read_text()
        marker = 'def main() -> int:\n    print("NEW COPY RAN", sys.argv[1:])\n    return 3\n\n\ndef old_main() -> int:'
        self.repo.commit({"tools/land_self.py": land.replace("def main() -> int:", marker, 1)},
                         "mainline changes land_self.py")
        r = self.land()
        self.assertEqual(r.returncode, 3, r.stdout + r.stderr)
        self.assertIn("starting again with the new copy", r.stdout)
        self.assertIn("NEW COPY RAN []", r.stdout)

    def test_no_restart_when_tools_unchanged(self):
        self.agent_commit()
        self.repo.commit({"other.txt": "x\n"}, "mainline change")
        r = self.land()
        self.assertNotIn("starting again", r.stdout + r.stderr)
        self.assertEqual(r.returncode, 2, r.stdout + r.stderr)  # replayed, then no Makefile for make release

    def test_keeps_queue_turn_across_restart(self):
        self.agent_commit()
        land = (TOOLS / "land_self.py").read_text()
        marker = ('def main() -> int:\n    fd = int(os.environ["LAND_QUEUE_FD"])\n'
                  '    print("QUEUE FD OPEN", os.fstat(fd).st_ino == QUEUE_LOCK.stat().st_ino)\n'
                  '    return 3\n\n\ndef old_main() -> int:')
        self.repo.commit({"tools/land_self.py": land.replace("def main() -> int:", marker, 1)},
                         "mainline changes land_self.py")
        r = self.land()
        self.assertEqual(r.returncode, 3, r.stdout + r.stderr)
        self.assertIn("QUEUE FD OPEN True", r.stdout)

    def test_review_stamp_help(self):
        r = subprocess.run([sys.executable, str(TOOLS / "review_stamp.py"), "--help"],
                           cwd=self.wt, capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("--missing", r.stdout)


class LandQueueTest(RepoCase):
    """2026-10-09: 11 agents landing at once each rebuilt after every other
    landing (each success moved mainline under the rest), so the work grew
    with the square of the agents. Landings now take turns."""

    def setUp(self):
        super().setUp()
        self.log = Path(self.tmp.name) / "release.log"
        tools = {f"tools/{t}": (TOOLS / t).read_text() for t in LandSelfReexecTest.TOOL_FILES}
        tools["tools/progress.py"] = "pass\n"
        # make release logs its start and end, and takes long enough to overlap.
        tools["Makefile"] = f"release:\n\techo start >> {self.log}\n\tsleep 1\n\techo end >> {self.log}\n"
        self.repo.commit(tools, "base")
        self.wts = []
        for name in ("a", "b"):
            wt = Path(self.tmp.name) / name
            self.repo.git("worktree", "add", "-q", "-b", name, str(wt))
            (wt / f"{name}.txt").write_text(f"{name}\n")
            self.repo.git("-C", str(wt), "add", "-A")
            self.repo.git("-C", str(wt), "commit", "-q", "-m", f"Add {name}")
            self.wts.append(wt)

    def land(self, wt: Path) -> subprocess.Popen:
        env = {k: v for k, v in os.environ.items() if k not in ("LAND_SELF_LOCK_HELD", "LAND_QUEUE_FD")}
        return subprocess.Popen([sys.executable, "tools/land_self.py"], cwd=wt, env=env, text=True,
                                stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    def test_second_landing_waits(self):
        first = self.land(self.wts[0])
        # Start the second run once the first one builds, so it has to wait.
        for _ in range(300):
            if self.log.exists():
                break
            time.sleep(0.1)
        second = self.land(self.wts[1])
        out = [p.communicate(timeout=120)[0] for p in (first, second)]
        self.assertEqual([first.returncode, second.returncode], [0, 0], "\n".join(out))
        self.assertIn("waiting for the landing queue, held by pid", out[1])
        self.assertNotIn("mainline moved", out[1])
        # One build at a time, one build each.
        self.assertEqual(self.log.read_text().split(), ["start", "end", "start", "end"])
        self.assertEqual(self.repo.git("log", "--format=%s", "-3", "mainline").splitlines(),
                         ["Add b", "Add a", "base"])


sys.path.insert(0, str(TOOLS))

if __name__ == "__main__":
    unittest.main()
