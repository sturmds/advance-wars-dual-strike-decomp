#!/usr/bin/env python3
"""Replay a decomp-function commit from another branch onto the current one.

    apply_commit.py COMMIT [--no-commit]

Agents work in parallel worktrees and each commits one function. A plain
cherry-pick conflicts whenever two agents edit neighbouring lines of
delinks.txt or symbols.txt, so this replays config changes by meaning:

- delinks.txt: file entries the commit added (or marked complete) are added
  here, sorted by address.
- symbols.txt: symbols the commit renamed are renamed here, matched by
  address, and the new names replace the old ones in src/ and include/.
- relocs.txt: lines the commit added are added here.
- Other files: written if this branch still has the parent's version,
  otherwise merged with `git merge-file`. A conflict stops the replay.

Then it commits with the original message and author, which runs the hooks
(so `make release` must pass).
"""

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TEXT = re.compile(r"\s+\.text\s+start:(0x[0-9a-f]+)")
SYM = re.compile(r"^(\S+) (kind:\S+ addr:(0x[0-9a-f]+).*)$")


def git(*args, check=True) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True, check=check).stdout


def show(rev: str, path: str) -> str | None:
    r = subprocess.run(["git", "show", f"{rev}:{path}"], cwd=ROOT, capture_output=True, text=True)
    return r.stdout if r.returncode == 0 else None


def delinks_blocks(text: str):
    head, _, rest = text.partition("\n\n")
    blocks = {}
    for b in rest.strip("\n").split("\n\n"):
        if b.strip():
            blocks[b.split("\n", 1)[0]] = b
    return head, blocks


def apply_delinks(path: str, old: str, new: str) -> None:
    _, old_blocks = delinks_blocks(old)
    _, new_blocks = delinks_blocks(new)
    head, cur_blocks = delinks_blocks((ROOT / path).read_text())
    for key, block in new_blocks.items():
        if old_blocks.get(key) != block:
            cur_blocks[key] = block

    def start(b):
        m = TEXT.search(b)
        return int(m.group(1), 16) if m else 0
    blocks = sorted(cur_blocks.values(), key=start)
    (ROOT / path).write_text(head + "\n\n" + "\n\n".join(blocks) + "\n")


def symbol_names(text: str) -> dict[tuple[str, str], str]:
    """(addr, rest-of-line without the name) -> name"""
    out = {}
    for line in text.splitlines():
        m = SYM.match(line)
        if m:
            out[(m.group(3), m.group(2))] = m.group(1)
    return out


def apply_symbols(path: str, old: str, new: str) -> list[tuple[str, str]]:
    before, after = symbol_names(old), symbol_names(new)
    renames = [(before[k], after[k]) for k in before if k in after and before[k] != after[k]]
    lines = (ROOT / path).read_text().splitlines()
    for old_name, new_name in renames:
        for i, line in enumerate(lines):
            if line.startswith(old_name + " "):
                lines[i] = new_name + line[len(old_name):]
                break
        else:
            sys.exit(f"error: {path} has no symbol {old_name} to rename to {new_name}")
    added = [ln for ln in new.splitlines() if ln not in set(old.splitlines())
             and not any(ln.startswith(n + " ") for _, n in renames)]
    existing = set(lines)
    lines += [ln for ln in added if ln not in existing]
    (ROOT / path).write_text("\n".join(lines) + "\n")
    return renames


def apply_relocs(path: str, old: str, new: str) -> None:
    old_set = set(old.splitlines())
    removed = old_set - set(new.splitlines())
    if removed:
        sys.exit(f"error: commit removes lines from {path}; replay it by hand")
    cur = (ROOT / path).read_text().splitlines()
    have = set(cur)
    cur += [ln for ln in new.splitlines() if ln not in old_set and ln not in have]
    (ROOT / path).write_text("\n".join(cur) + "\n")


def merge_file(path: str, base: str | None, theirs: str | None) -> None:
    target = ROOT / path
    if theirs is None:
        if target.exists() and base is not None and target.read_text() == base:
            target.unlink()
        return
    if not target.exists():
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(theirs)
        return
    ours = target.read_text()
    if ours == theirs:
        return
    if base is not None and ours == base:
        target.write_text(theirs)
        return
    tmp = ROOT / "build" / "apply_commit"
    tmp.mkdir(parents=True, exist_ok=True)
    (tmp / "base").write_text(base or "")
    (tmp / "theirs").write_text(theirs)
    r = subprocess.run(["git", "merge-file", "-L", "current", "-L", "base", "-L", "commit",
                        str(target), str(tmp / "base"), str(tmp / "theirs")], cwd=ROOT)
    if r.returncode:
        sys.exit(f"error: conflict in {path}; resolve it, then commit by hand")


def rename_in_sources(renames) -> None:
    files = list((ROOT / "src").rglob("*.[ch]")) + list((ROOT / "include").rglob("*.h"))
    for old, new in renames:
        word = re.compile(rf"(?<!\w){re.escape(old)}(?!\w)")
        for p in files:
            text = p.read_text()
            if word.search(text):
                p.write_text(word.sub(new, text))


def main() -> int:
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    commit = git("rev-parse", sys.argv[1]).strip()
    if git("status", "--porcelain", "--untracked-files=no").strip():
        sys.exit("error: working tree has uncommitted changes")
    changes = git("diff-tree", "--no-commit-id", "-r", "--name-status", f"{commit}^", commit).splitlines()
    renames = []
    for line in changes:
        status, path = line.split("\t", 1)
        if status[0] not in "AMD":
            sys.exit(f"error: unsupported change {status} {path}")
        old = show(f"{commit}^", path)
        new = show(commit, path)
        name = Path(path).name
        if path.startswith("config/") and name == "delinks.txt":
            apply_delinks(path, old, new)
        elif path.startswith("config/") and name == "symbols.txt":
            renames += apply_symbols(path, old, new)
        elif path.startswith("config/") and name == "relocs.txt":
            apply_relocs(path, old, new)
        else:
            merge_file(path, old, new)
    rename_in_sources(renames)
    for old, new in renames:
        print(f"renamed {old} -> {new}")

    git("add", "-A", "config", "src", "include", "docs")
    if "--no-commit" in sys.argv:
        print("applied; not committed")
        return 0
    r = subprocess.run(["git", "commit", "-q", "-C", commit], cwd=ROOT)
    if r.returncode:
        print("error: commit failed (hooks?). Changes are staged; fix and commit by hand.", file=sys.stderr)
        return 1
    print(f"applied {commit[:10]} as {git('rev-parse', '--short', 'HEAD').strip()}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
