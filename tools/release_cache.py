#!/usr/bin/env python3
"""Skip `make release` on a tree that has already passed it.

    release_cache.py release [--report] [--label NAME]
    release_cache.py begin | record
    release_cache.py test SCRIPT...

hooks/pre-commit runs `make release` on every commit (but those that
land_self.py replays), and land_self.py runs it on the final tree of a
landing, which the agent's last commit often checked already. This tool
remembers the trees that passed.

The key of a tree: `git write-tree` of the index with PROGRESS.md and
PROGRESS.json left out (only the build writes them). make release builds the
working tree, not the index, so a tree has a key only when the working tree
equals the index for every tracked file but those two, no file is
unmerged, and no untracked file is left outside .gitignore. Otherwise
nothing is looked up or recorded and make release runs as before. In a hook,
git sets GIT_INDEX_FILE to the index being committed, so the key is the
tree of the commit.

The cache lives in the git common directory, shared by the main checkout
and every worktree, outside any work tree, so git never tracks it:
<git common dir>/release-ok/<key> for releases and
<git common dir>/test-ok/<key> for tests. Delete those directories to empty
it.

release      Run `make release` unless the tree's key is in the cache. With
             --report, also make sure build/artifacts/AWRE_report/report.json
             is for this tree (pre-commit in the main checkout and
             land_self.py write PROGRESS.md from it): if build/report-tree
             does not name this key and the report's SHA-256, it runs `make
             decomp-dev-report`, and the full release if that fails.
begin        First step of `make release`: write the key, if any, to
             build/release-start.
record       Last step of `make release`, reached only when every step
             passed: if the key is still the one `begin` wrote, add it to
             the cache and write build/report-tree.
test         Run each SCRIPT (with the GIT_ variables removed, as TEST_ENV in
             the Makefile does) unless the same scripts already passed with
             the same tools/, Makefile, tools.lock and docs/style/. Only
             `make release` uses this; `make test` runs every test.

RELEASE_CACHE=off in the environment turns off every lookup: release and
test run in full (a pass is still recorded).
"""

import argparse
import hashlib
import os
import subprocess
import sys
import tempfile
from pathlib import Path

PROGRESS = ("PROGRESS.md", "PROGRESS.json")
# What the cached tests read from the checkout, besides the tools from tools.lock.
TEST_INPUTS = ("tools", "Makefile", "tools.lock", "docs/style")
REPORT = Path("build/artifacts/AWRE_report/report.json")
REPORT_STAMP = Path("build/report-tree")
START = Path("build/release-start")


def git(*args: str, env: dict | None = None, data: bytes | None = None) -> subprocess.CompletedProcess:
    return subprocess.run(["git", *args], capture_output=True, input=data, env=env)


def out(*args: str) -> str:
    r = git(*args)
    if r.returncode:
        sys.exit(f"release_cache.py: git {' '.join(args)}: {r.stderr.decode().strip()}")
    return r.stdout.decode().strip()


ROOT = Path(out("rev-parse", "--show-toplevel"))
COMMON = Path(out("rev-parse", "--path-format=absolute", "--git-common-dir"))
os.chdir(ROOT)


def enabled() -> bool:
    return os.environ.get("RELEASE_CACHE", "").lower() != "off"


def clean(*paths: str) -> bool:
    """True if the working tree equals the index under paths (all when
    none), leaving out PROGRESS.*, with nothing unmerged or untracked."""
    spec = ["--", *(paths or ["."]), *(f":(exclude){p}" for p in PROGRESS)]
    if git("diff", "--quiet", *spec).returncode:
        return False
    if git("ls-files", "--unmerged", *spec).stdout:
        return False
    return not git("ls-files", "--others", "--exclude-standard", *spec).stdout


def tree_key() -> str | None:
    """The key of the tree make release builds, or None (see the top)."""
    if not clean():
        return None
    entries = [e for e in git("ls-files", "--stage", "-z").stdout.split(b"\0")
               if e and e.split(b"\t", 1)[1].decode() not in PROGRESS]
    with tempfile.TemporaryDirectory() as tmp:
        env = {**os.environ, "GIT_INDEX_FILE": os.path.join(tmp, "index")}
        if git("update-index", "-z", "--index-info", env=env, data=b"\0".join(entries) + b"\0").returncode:
            return None
        r = git("write-tree", env=env)
        return r.stdout.decode().strip() if r.returncode == 0 else None


def test_key(scripts: list[str]) -> str | None:
    if not clean(*TEST_INPUTS):
        return None
    listing = git("ls-files", "--stage", "-z", "--", *TEST_INPUTS).stdout
    return hashlib.sha256(listing + "\0".join(scripts).encode()).hexdigest()


def cached(kind: str, key: str | None) -> bool:
    return bool(key) and enabled() and (COMMON / kind / key).is_file()


def remember(kind: str, key: str, note: str) -> None:
    """Add key to the cache: write a temporary file, then rename it."""
    d = COMMON / kind
    d.mkdir(exist_ok=True)
    fd, tmp = tempfile.mkstemp(dir=d, prefix=".tmp-")
    with os.fdopen(fd, "w") as f:
        f.write(note + "\n")
    os.replace(tmp, d / key)


def report_sha() -> str:
    return hashlib.sha256(REPORT.read_bytes()).hexdigest() if REPORT.exists() else ""


def stamp_report(key: str) -> None:
    REPORT_STAMP.write_text(f"{key} {report_sha()}\n")


def report_is_for(key: str) -> bool:
    try:
        return REPORT_STAMP.read_text().split() == [key, report_sha()]
    except OSError:
        return False


def make(*targets: str) -> int:
    return subprocess.run(["make", *targets]).returncode


def cmd_release(args) -> int:
    key = tree_key()
    if not cached("release-ok", key):
        return make("release")
    print(f"{args.label}: tree {key[:10]} already passed make release; skipping it", flush=True)
    if args.report and not report_is_for(key):
        print(f"{args.label}: the decomp.dev report is not for this tree; running make decomp-dev-report",
              flush=True)
        if make("decomp-dev-report"):
            print(f"{args.label}: make decomp-dev-report failed; running make release", flush=True)
            return make("release")
        if tree_key() != key:
            return make("release")
        stamp_report(key)
    return 0


def cmd_begin(args) -> int:
    START.unlink(missing_ok=True)
    key = tree_key()
    if key:
        START.parent.mkdir(exist_ok=True)
        START.write_text(key + "\n")
    return 0


def cmd_record(args) -> int:
    try:
        started = START.read_text().strip()
    except OSError:
        return 0
    START.unlink()
    if started and tree_key() == started:
        remember("release-ok", started, "make release passed")
        stamp_report(started)
    return 0


def cmd_test(args) -> int:
    key = test_key(args.scripts)
    names = " ".join(Path(s).name for s in args.scripts)
    if cached("test-ok", key):
        print(f"test: {names} already passed with these tools/, Makefile, tools.lock and docs/style/;"
              " skipping it (make test runs it)", flush=True)
        return 0
    # As TEST_ENV in the Makefile: git's hook variables would send the tests'
    # git commands to this repository (issue 0076).
    env = {k: v for k, v in os.environ.items() if not k.startswith("GIT_") or k == "GIT_EDITOR"}
    for s in args.scripts:
        if subprocess.run([sys.executable, s], env=env).returncode:
            return 1
    if key and test_key(args.scripts) == key:
        remember("test-ok", key, "passed: " + names)
    return 0


def main() -> int:
    p = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = p.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("release")
    r.add_argument("--report", action="store_true", help="make sure the decomp.dev report is for this tree")
    r.add_argument("--label", default="release_cache", help="name at the start of the message")
    sub.add_parser("begin")
    sub.add_parser("record")
    t = sub.add_parser("test")
    t.add_argument("scripts", nargs="+")
    args = p.parse_args()
    return {"release": cmd_release, "begin": cmd_begin, "record": cmd_record, "test": cmd_test}[args.cmd](args)


if __name__ == "__main__":
    sys.exit(main())
