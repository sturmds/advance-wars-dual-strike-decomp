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
- PROGRESS.md: skipped; the pre-commit hook writes it again here.
- Other files: written if this branch still has the parent's version,
  otherwise merged with `git merge-file`. A conflict in a header under
  include/ (also a header both sides added) is merged by meaning with
  tools/header_merge.py: struct fields by offset with the padding written
  again, other declarations by name, mainline's names and versions kept on
  a clash. The commit's sources are then rewritten to mainline's field and
  type names. Any other conflict stops the replay after the rest of the
  commit is applied.
- Source files: the commit's symbol renames are applied to both older
  versions before the merge. Afterwards, local prototypes of functions an
  included header declares are dropped from the commit's .c files, and
  conflict blocks whose two sides have become equal are resolved.

Then it commits with the original message and author, which runs the hooks
(so `make release` must pass).
"""

import re
import subprocess
import sys
from pathlib import Path

import header_merge

# The checkout the command runs in, which is not always the one holding this
# script: tests run a copy of it against other commits.
ROOT = Path(subprocess.run(["git", "rev-parse", "--show-toplevel"], capture_output=True, text=True,
                           check=True).stdout.strip())
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
    for key in old_blocks.keys() - new_blocks.keys():
        cur_blocks.pop(key, None)

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


def merge_file(path: str, base: str | None, theirs: str | None, renames: dict[str, str]) -> None:
    """Write the commit's version of a file, merged with this branch's.
    renames: the commit's symbol renames, applied to both older versions
    first so that a renamed call does not conflict with a nearby edit."""
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
    if renames and path.startswith(("src/", "include/")):
        base = header_merge.rename_words(base, renames) if base else base
        ours = header_merge.rename_words(ours, renames)
        target.write_text(ours)
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
        if path.startswith("include/") and merge_header(path, ours, theirs, base or ""):
            return
        CONFLICTS.append(path)


CONFLICTS: list[str] = []
CONFLICT_BLOCK = re.compile(r"^<<<<<<< current\n(.*?)^=======\n(.*?)^>>>>>>> commit\n", re.M | re.S)
MERGED_HEADERS: dict[str, header_merge.Report] = {}
COMMIT = ""


def merge_header(path: str, ours: str, theirs: str, base: str) -> bool:
    """Merge a conflicting header by meaning (tools/header_merge.py). False
    if it cannot be read; the conflict markers of git merge-file then stay."""
    try:
        text, rep = header_merge.merge(ours, theirs, base, header_types(path, ours, theirs))
    except header_merge.MergeError as e:
        print(f"{path}: cannot merge by meaning: {e}")
        return False
    (ROOT / path).write_text(text)
    print(rep.summary(path))
    MERGED_HEADERS[path] = rep
    for c in rep.conflicts:
        CONFLICTS.append(f"{path} ({c})")
    return True


def header_types(path: str, ours: str, theirs: str) -> header_merge.Types:
    """Type sizes from the headers of both sides, to lay out struct fields."""
    texts = [ours, theirs]
    for p in sorted((ROOT / "include").rglob("*.h")):
        if p != ROOT / path:
            texts.append(p.read_text())
    for name in git("diff-tree", "--no-commit-id", "-r", "--name-only", f"{COMMIT}^", COMMIT).splitlines():
        if name.startswith("include/") and name != path:
            texts.append(show(COMMIT, name) or "")
    return header_merge.Types(texts)


def member_access(name: str) -> re.Pattern:
    return re.compile(rf"(->|\.)(\s*){re.escape(name)}(?!\w)")


def apply_header_renames(paths: list[str]) -> None:
    """After a header merge kept mainline's field and type names, rewrite
    the commit's sources to use them. Fields match only as member accesses
    (->name, .name), but of any struct: a wrong rename fails the build
    rather than going unseen."""
    fields, types = {}, {}
    for rep in MERGED_HEADERS.values():
        for old, new in rep.renames.items():
            if fields.setdefault(old, new) != new:
                print(f"warning: {old} maps to both {fields[old]} and {new}; renamed to {fields[old]}")
        types.update(rep.type_renames)
    for path in paths:
        if path.endswith(".c") or (path.endswith(".h") and path not in MERGED_HEADERS):
            rewrite(path, fields, types)


def rewrite(path: str, fields: dict[str, str], types: dict[str, str]) -> None:
    p = ROOT / path
    if not p.exists():
        return
    text = new = p.read_text()
    for old, name in fields.items():
        new = member_access(old).sub(rf"\g<1>\g<2>{name}", new)
    new = header_merge.rename_words(new, types)
    if new != text:
        p.write_text(new)
        print(f"renamed fields and types in {path} to the merged header's names")


INCLUDE = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)


def included(path: Path) -> list[Path]:
    """Headers a file includes, directly and one level down."""
    out = []
    for name in INCLUDE.findall(path.read_text()):
        h = ROOT / "include" / name
        if h.exists():
            out.append(h)
            out += [ROOT / "include" / n for n in INCLUDE.findall(h.read_text()) if (ROOT / "include" / n).exists()]
    return out


PROTOTYPE = re.compile(r"^(?:extern[ \t]+)?[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{}]*\)\s*;[ \t]*(?:/\*.*\*/)?[ \t]*\n", re.M)


def drop_header_prototypes(paths: list[str]) -> None:
    """Remove local prototypes of functions that an included header declares.
    Agents declare callees at the top of their file until a header has
    them; a 3-way merge brings such lines back after a header gained the
    function. The header's version wins, also when the parameter types differ."""
    for path in paths:
        p = ROOT / path
        if not (path.endswith(".c") and p.exists()):
            continue
        declared = set()
        for h in included(p):
            declared |= header_merge.declared_functions(h.read_text())
        text = p.read_text()
        new = PROTOTYPE.sub(lambda m: "" if m.group(1) in declared else m.group(0), text)
        if new != text:
            p.write_text(re.sub(r"\n\n\n+", "\n\n", new))
            print(f"dropped prototypes in {path} that its headers declare")


def rename_in_sources(renames) -> None:
    files = list((ROOT / "src").rglob("*.[ch]")) + list((ROOT / "include").rglob("*.h"))
    for old, new in renames:
        word = re.compile(rf"(?<!\w){re.escape(old)}(?!\w)")
        for p in files:
            text = p.read_text()
            if word.search(text):
                p.write_text(word.sub(new, text))


ADDR_NAME = re.compile(r"\b(?:func|data)_(?:(ov\d{3})_)?([0-9a-f]{8})\b")


def fix_stale_names(paths) -> None:
    """A commit made in parallel may still use an address name (func_020e82a8)
    that another landed commit has since renamed. Replace each address name
    that is no longer a symbol with the symbol now at that address."""
    names = {}
    for sym in (ROOT / "config/AWRE/arm9").rglob("symbols.txt"):
        module = sym.parent.name if sym.parent.parent.name == "overlays" else None
        for line in sym.read_text().splitlines():
            m = SYM.match(line)
            if m:
                names.setdefault((module, m.group(3)), m.group(1))
    current = set(names.values())
    for path in paths:
        p = ROOT / path
        if not (path.startswith(("src/", "include/")) and p.exists()):
            continue
        text = p.read_text()

        def sub(m):
            if m.group(0) in current:
                return m.group(0)
            return names.get((m.group(1), "0x" + m.group(2)), m.group(0))
        new_text = ADDR_NAME.sub(sub, text)
        if new_text != text:
            p.write_text(new_text)
            print(f"updated stale address names in {path}")


def resolve_equal_sides() -> None:
    """Take conflict blocks whose two sides became equal once names were
    updated and prototypes dropped."""
    for path in list(CONFLICTS):
        p = ROOT / path
        if not p.is_file():
            continue
        text = p.read_text()
        new = CONFLICT_BLOCK.sub(lambda m: m.group(1) if m.group(1) == m.group(2) else m.group(0), text)
        if new != text:
            p.write_text(new)
        if not CONFLICT_BLOCK.search(new):
            CONFLICTS.remove(path)
            print(f"merged {path}: both sides of each conflict were equal after the renames")


def main() -> int:
    if len(sys.argv) < 2:
        sys.exit(__doc__)
    global COMMIT
    commit = COMMIT = git("rev-parse", sys.argv[1]).strip()
    if git("status", "--porcelain", "--untracked-files=no").strip():
        sys.exit("error: working tree has uncommitted changes")
    changes = git("diff-tree", "--no-commit-id", "-r", "--name-status", f"{commit}^", commit).splitlines()
    renames = []
    for line in changes:
        status, path = line.split("\t", 1)
        if path == "PROGRESS.md":
            continue  # the pre-commit hook writes it for this branch
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
            merge_file(path, old, new, dict(renames))
    rename_in_sources(renames)
    for old, new in renames:
        print(f"renamed {old} -> {new}")
    paths = [ln.split("\t", 1)[1] for ln in changes]
    apply_header_renames(paths)
    fix_stale_names(paths)
    drop_header_prototypes(paths)
    resolve_equal_sides()
    if CONFLICTS:
        sys.exit("error: conflict in " + ", ".join(CONFLICTS) + "; resolve, then commit by hand")

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
