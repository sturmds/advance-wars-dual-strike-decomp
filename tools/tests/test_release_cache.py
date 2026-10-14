#!/usr/bin/env python3
"""Check the release cache (tools/release_cache.py) and hooks/pre-commit on small repos.

    python3 tools/tests/test_release_cache.py     (or: make test)

Each case builds a throwaway git repo with this checkout's pre-commit hook
and release_cache.py, and a stub Makefile whose release, test and report
steps write a line each to build/log. Nothing in this checkout is touched,
and no ROM or build is needed.
"""

import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

# Git exports GIT_DIR, GIT_INDEX_FILE and others to hooks, and make exports
# MAKEFLAGS. Run from a hook, the git commands below would go to the
# repository being committed (issue 0076).
for _name in [n for n in os.environ if n.startswith("GIT_") and n != "GIT_EDITOR"
              or n in ("MAKEFLAGS", "MFLAGS", "MAKELEVEL", "RELEASE_CACHE", "LAND_SELF_REPLAY")]:
    del os.environ[_name]

TOOLS = Path(__file__).resolve().parent.parent

MAKEFILE = """\
release:
\t@python3 tools/release_cache.py begin
\t@mkdir -p build && echo release >> build/log
\t@$(MAKE) --no-print-directory test-release
\t@$(MAKE) --no-print-directory decomp-dev-report
\t@python3 tools/release_cache.py record

test-release:
\t@python3 tools/release_cache.py test tools/tests/t.py

decomp-dev-report:
\t@mkdir -p build/artifacts/AWRE_report && echo report >> build/log
\t@cat *.txt > build/artifacts/AWRE_report/report.json
"""

TEST_SCRIPT = "open('build/log', 'a').write('test\\n')\n"

# The hook writes PROGRESS.md and PROGRESS.json from the report.
PROGRESS = """\
import shutil
for p in ("PROGRESS.md", "PROGRESS.json"):
    shutil.copy("build/artifacts/AWRE_report/report.json", p)
"""


class Case(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.path = Path(self.tmp.name) / "repo"
        self.path.mkdir()
        self.env = {**os.environ, "GIT_CONFIG_GLOBAL": os.devnull, "GIT_CONFIG_NOSYSTEM": "1", "TZ": "UTC"}
        self.git("init", "-q", "-b", "mainline")
        gitdir = Path(self.git("rev-parse", "--absolute-git-dir")).resolve()
        if not gitdir.is_relative_to(self.path.resolve()):
            print(f"test_release_cache.py: git resolves {self.path} to {gitdir}, outside the test's "
                  "directory; stopping before any test writes there", file=sys.stderr)
            os._exit(3)
        self.git("config", "user.name", "test")
        self.git("config", "user.email", "test@example.com")
        self.git("config", "core.hooksPath", "hooks")
        self.write(".gitignore", "build/\n__pycache__/\n")
        self.write("Makefile", MAKEFILE)
        self.write("hooks/pre-commit", (TOOLS.parent / "hooks/pre-commit").read_text())
        (self.path / "hooks/pre-commit").chmod(0o755)
        self.write("tools/release_cache.py", (TOOLS / "release_cache.py").read_text())
        self.write("tools/progress.py", PROGRESS)
        self.write("tools/tests/t.py", TEST_SCRIPT)
        self.write("a.txt", "one\n")
        self.commit("base")

    def tearDown(self):
        self.tmp.cleanup()

    def git(self, *args: str, cwd: Path | None = None) -> str:
        return subprocess.run(["git", *args], cwd=cwd or self.path, capture_output=True, text=True,
                              check=True, env=self.env).stdout.strip()

    def write(self, rel: str, text: str, root: Path | None = None) -> None:
        p = (root or self.path) / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text)

    def commit(self, msg: str, cwd: Path | None = None, env: dict | None = None) -> str:
        """git add -A (unless msg starts with '-'), commit, return the hook's output."""
        cwd = cwd or self.path
        if not msg.startswith("-"):
            self.git("add", "-A", cwd=cwd)
        r = subprocess.run(["git", "commit", "-q", "--allow-empty", "-m", msg.lstrip("-")], cwd=cwd,
                           capture_output=True, text=True, env={**self.env, **(env or {})})
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        return r.stdout + r.stderr

    def log(self, root: Path | None = None) -> list[str]:
        """The steps run since the last call."""
        p = (root or self.path) / "build/log"
        steps = p.read_text().split() if p.exists() else []
        p.unlink(missing_ok=True)
        return steps

    def test_same_tree_skips(self):
        self.assertEqual(self.log(), ["release", "test", "report"])
        out = self.commit("same tree")
        self.assertEqual(self.log(), [])
        tree = self.git("rev-parse", "HEAD^{tree}")[:10]
        self.assertNotIn(tree, out)  # the key leaves PROGRESS.* out, so it differs from the commit's tree
        self.assertRegex(out, r"pre-commit: tree [0-9a-f]{10} already passed make release; skipping it")

    def test_changed_tree_runs(self):
        self.log()
        self.write("a.txt", "two\n")
        self.commit("change a")
        self.assertEqual(self.log(), ["release", "report"])  # the tests' inputs did not change
        self.assertEqual((self.path / "PROGRESS.md").read_text(), "two\n")

    def test_unstaged_or_untracked_disables(self):
        self.log()
        self.write("a.txt", "unstaged\n")
        self.commit("-unstaged change")
        self.assertEqual(self.log(), ["release", "report"])
        self.git("checkout", "a.txt")
        self.write("new.txt", "x\n")
        self.commit("-untracked file")
        self.assertEqual(self.log(), ["release", "report"])
        (self.path / "new.txt").unlink()
        self.commit("-clean again")
        self.assertEqual(self.log(), ["report"])  # a hit: neither run was recorded; the report was not for this tree

    def test_progress_alone_hits(self):
        self.log()
        self.write("PROGRESS.md", "edited by hand\n")
        out = self.commit("progress only")
        self.assertIn("skipping it", out)
        self.assertEqual(self.log(), [])
        self.write("PROGRESS.json", "unstaged\n")
        self.assertIn("skipping it", self.commit("-progress unstaged"))

    def test_cache_outside_work_tree(self):
        self.commit("again")
        common = Path(self.git("rev-parse", "--path-format=absolute", "--git-common-dir"))
        self.assertEqual(len(list((common / "release-ok").iterdir())), 1)
        self.assertEqual(len(list((common / "test-ok").iterdir())), 1)
        self.assertEqual(self.git("status", "--porcelain", "--ignored=no"), "")

    def test_off_runs(self):
        self.log()
        out = self.commit("off", env={"RELEASE_CACHE": "off"})
        self.assertNotIn("skipping", out)
        self.assertEqual(self.log(), ["release", "test", "report"])

    def test_test_cache(self):
        self.log()
        for path in ("tools/x.py", "Makefile", "tools.lock", "docs/style/banned.txt"):
            self.write(path, (self.path / path).read_text() + "#\n" if (self.path / path).exists() else "x\n")
            self.commit(f"change {path}")
            self.assertEqual(self.log(), ["release", "test", "report"], path)
        self.write("src.c", "int x;\n")
        self.commit("change src")
        self.assertEqual(self.log(), ["release", "report"])

    def test_tree_from_worktree_counts(self):
        """A tree checked in a linked worktree skips the release in the main
        checkout, and the report is rebuilt for PROGRESS.md."""
        self.log()
        wt = Path(self.tmp.name) / "wt"
        self.git("worktree", "add", "-q", "-b", "agent", str(wt))
        self.write("c.txt", "three\n", wt)
        self.commit("add c", cwd=wt)
        self.assertEqual(self.log(wt), ["release", "report"])
        self.assertFalse((wt / "PROGRESS.md").read_text() == "one\nthree\n")  # not written in a worktree
        self.write("c.txt", "three\n")
        out = self.commit("add c in the main checkout")
        self.assertIn("skipping it", out)
        self.assertEqual(self.log(), ["report"])
        self.assertEqual((self.path / "PROGRESS.md").read_text(), "one\nthree\n")
        self.assertEqual(self.git("status", "--porcelain"), "")


if __name__ == "__main__":
    unittest.main()
