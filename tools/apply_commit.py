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
  otherwise merged with `git merge-file`. A conflict in a header under
  include/ is merged by meaning: struct fields by offset (padding
  regenerated), #define and prototype lines by union. Any other conflict
  stops the replay after the rest of the commit is applied.

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


# Sizes of the types used in shared structs, for merging padding by offset.
TYPE_SIZES = {"u8": 1, "s8": 1, "char": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4,
              "int": 4, "BOOL": 4, "fx32": 4, "TextRef": 4}
FIELD = re.compile(r"^\s*(?:const\s+)?(\w+)\s*(\*?)\s*(\w+)((?:\[[^\]]+\])*)\s*;")
STRUCT = re.compile(r"(typedef struct (\w+) \{\n)(.*?)(\} \2;)", re.S)


def struct_fields(body: str):
    """[(offset, size, line)] of the named fields, and the struct size.
    Raises ValueError on anything it cannot size (nested structs, unions)."""
    off, fields = 0, []
    for line in body.splitlines(True):
        m = FIELD.match(line)
        if not m:
            if line.strip() and not line.strip().startswith(("/*", "*", "//")):
                raise ValueError(line)
            continue
        typ, ptr, name, dims = m.groups()
        n = 4 if ptr else TYPE_SIZES.get(typ)
        if n is None:
            raise ValueError(line)
        for d in re.findall(r"\[([^\]]+)\]", dims):
            n *= eval(d, {})
        if not name.startswith("pad_"):
            fields.append((off, n, line))
        off += n
    return fields, off


def merge_struct(ours: str, theirs: str) -> str:
    """Keep every named field of both versions (ours wins where they
    overlap) and regenerate the pad_ arrays between them."""
    fa, ea = struct_fields(ours)
    fb, eb = struct_fields(theirs)
    out, taken = list(fa), [(o, o + n) for o, n, _ in fa]
    for o, n, line in fb:
        if not any(o < e and s < o + n for s, e in taken):
            out.append((o, n, line))
            taken.append((o, o + n))
    lines, cur = [], 0
    for o, n, line in sorted(out) + [(max(ea, eb), 0, "")]:
        if o > cur:
            lines.append(f"    u8 pad_{cur:02x}[0x{o:02x} - 0x{cur:02x}];\n" if cur else f"    u8 pad_00[0x{o:x}];\n")
        lines.append(line)
        cur = max(cur, o + n)
    return "".join(lines)


def merge_header(ours: str, theirs: str) -> str | None:
    """Merge a shared header that both sides only added to: structs by field
    offset, #define/#include/prototype lines by union (SKILL_ ids sorted).
    None if a changed struct cannot be parsed."""
    res = ours
    so = {m.group(2): m for m in STRUCT.finditer(ours)}
    for m in STRUCT.finditer(theirs):
        mine = so.get(m.group(2))
        if mine is None or mine.group(3) == m.group(3):
            continue
        try:
            body = merge_struct(mine.group(3), m.group(3))
        except ValueError:
            return None
        res = res.replace(mine.group(0), mine.group(1) + body + mine.group(4))
    flat = re.compile(r"^(#define|#include|\w[\w\s\*]*\(.*\);$)")
    for line in theirs.splitlines(True):
        if flat.match(line) and line not in res:
            anchor = res.rfind("\n#endif")
            res = res[:anchor] + "\n" + line.rstrip("\n") + res[anchor:]
    lines = res.splitlines(True)
    ids = [ln for ln in lines if ln.startswith("#define SKILL_")]
    if ids:
        first = next(i for i, ln in enumerate(lines) if ln.startswith("#define SKILL_"))
        rest = [ln for ln in lines if not ln.startswith("#define SKILL_")]
        seen, uniq = set(), []
        for ln in sorted(ids, key=lambda ln: int(ln.split()[2], 0)):
            if ln.split()[1] not in seen:
                seen.add(ln.split()[1])
                uniq.append(ln)
        res = "".join(rest[:first] + uniq + rest[first:])
    return res.replace("\n\n\n", "\n\n")


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
        if path.startswith("include/"):
            merged = merge_header(ours, theirs)
            if merged is not None:
                target.write_text(merged)
                print(f"merged {path} by struct offset")
                return
        CONFLICTS.append(path)


CONFLICTS: list[str] = []


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
    fix_stale_names([ln.split("\t", 1)[1] for ln in changes])
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
