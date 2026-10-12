#!/usr/bin/env python3
"""Replay a decomp-function commit from another branch onto the current one.

    apply_commit.py COMMIT [--no-commit]

Agents work in parallel worktrees and each commits one function. A plain
cherry-pick conflicts whenever two agents edit neighbouring lines of
delinks.txt or symbols.txt, so this replays config changes by meaning:

- delinks.txt: file entries the commit added (or marked complete) are added
  here, sorted by address; entries it removed are removed, and a change
  to the section list at the top is taken.
- symbols.txt: symbols the commit renamed are renamed here, matched by
  address, and the new names replace the old ones in src/ and include/.
  Other symbols the commit added, removed or changed are replayed by name.
- relocs.txt: relocations the commit added, removed or changed are
  replayed, matched by their from: address. An entry this branch has with
  other content is a conflict.
- PROGRESS.md: skipped; the pre-commit hook writes it again here.
- Other files: written if this branch still has the parent's version,
  otherwise merged with `git merge-file`. A conflict in a header under
  include/ (also a header both sides added) is merged by meaning with
  tools/header_merge.py: struct fields by offset with the padding written
  again, other declarations by name, mainline's names and versions kept on
  a clash. The commit's sources are then rewritten to mainline's field and
  type names; a field rename whose old name another struct also uses is
  not applied but reported, since `->name` does not say which struct it
  reads. Any other conflict stops the replay after the rest of the commit
  is applied.
- Deleted files (also the old path of a move): deleted when this branch's
  copy equals the parent's apart from renamed names, otherwise a conflict.
  An edit to a file this branch no longer has (mainline moved it) is
  skipped when it only applies the commit's symbol renames, otherwise a
  conflict; it never writes the old path again.
- Source files: the commit's symbol renames are applied to both older
  versions before the merge, and to the sources the commit did not touch.
  All renames apply in one pass, so a commit that renames A to B and gives
  A's old name to C does not turn C into B. Afterwards, local prototypes of functions an
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
    old_head, old_blocks = delinks_blocks(old)
    new_head, new_blocks = delinks_blocks(new)
    head, cur_blocks = delinks_blocks((ROOT / path).read_text())
    if new_head != old_head:
        if head == old_head:
            head = new_head
        elif head != new_head:
            CONFLICTS.append(f"{path} (the commit and this branch both changed the section list at the top)")
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
    renamed_keys = [k for k in before if k in after and before[k] != after[k]]
    renames = [(before[k], after[k]) for k in renamed_keys]
    lines = (ROOT / path).read_text().splitlines()
    # Find every line first, by name and address, then rename: a commit can
    # give one symbol's old name to another (A -> B, then C -> A).
    at = []
    for k in renamed_keys:
        old_name, new_name = before[k], after[k]
        for i, line in enumerate(lines):
            m = SYM.match(line)
            if m and m.group(1) == old_name and m.group(3) == k[0]:
                at.append((i, old_name, new_name))
                break
        else:
            sys.exit(f"error: {path} has no symbol {old_name} at {k[0]} to rename to {new_name}")
    for i, old_name, new_name in at:
        lines[i] = new_name + lines[i][len(old_name):]
    (ROOT / path).write_text("\n".join(lines) + "\n")
    renamed = {n for n, _ in renames} | {n for _, n in renames}
    replay_lines(path, old, new, lambda ln: ln.split(" ", 1)[0], skip=renamed)
    return renames


RELOC_FROM = re.compile(r"\bfrom:(0x[0-9a-f]+)")


def reloc_key(line: str) -> str:
    m = RELOC_FROM.search(line)
    return m.group(1) if m else line


def apply_relocs(path: str, old: str, new: str) -> None:
    replay_lines(path, old, new, reloc_key)


def replay_lines(path: str, old: str, new: str, key, skip: set[str] = frozenset()) -> None:
    """Replay the lines the commit removed, added or changed in a config
    file of one entry per line. key(line) names the entry (a symbol's name,
    a relocation's from: address); a changed entry is replaced where it
    stands. Lines whose key is in skip were handled already (renames)."""
    old_lines, new_lines = old.splitlines(), new.splitlines()
    removed = {ln for ln in set(old_lines) - set(new_lines) if key(ln) not in skip}
    added = {key(ln): ln for ln in new_lines if ln not in set(old_lines) and key(ln) not in skip}
    cur = (ROOT / path).read_text().splitlines()
    for ln in sorted(removed - set(cur)):
        print(f"{path}: `{ln}` is already gone here")
    out = []
    for ln in cur:
        if ln not in removed:
            out.append(ln)
        elif key(ln) in added:
            out.append(added.pop(key(ln)))
    have = {key(ln): ln for ln in out}
    for k, ln in added.items():
        if k not in have:
            out.append(ln)
        elif have[k] != ln:
            CONFLICTS.append(f"{path} (the commit has `{ln}`, this branch has `{have[k]}`)")
    (ROOT / path).write_text("\n".join(out) + "\n")


def merge_file(path: str, base: str | None, theirs: str | None, renames: dict[str, str]) -> None:
    """Write the commit's version of a file, merged with this branch's.
    renames: the commit's symbol renames, applied to both older versions
    first so that a renamed call does not conflict with a nearby edit."""
    target = ROOT / path
    code = path.startswith(("src/", "include/"))
    if theirs is None:
        if not target.exists() or base is None:
            return
        ours = target.read_text()
        if code:
            base, ours = header_merge.rename_words(base, renames), header_merge.rename_words(ours, renames)
        if ours == base or (code and names_only_differ(base, ours)):
            # Also when this branch renamed names in it since (another
            # landed commit's renames): otherwise a moved file stays twice.
            target.unlink()
        else:
            CONFLICTS.append(f"{path} (the commit deletes it, but this branch changed it since;"
                             " carry the change to where the commit moved it, then delete it)")
        return
    if not target.exists():
        if base is None:
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_text(theirs)
        elif code and header_merge.rename_words(base, renames) == theirs:
            # This branch deleted or moved the file; rename_in_sources gives
            # the file at its new path the same renames.
            print(f"skipped {path}: this branch no longer has it, and the commit only renamed symbols in it")
        else:
            moved = find_copies(path, base) if code else []
            CONFLICTS.append(f"{path} (the commit edits it, but this branch deleted or moved it"
                             + (f", perhaps to {', '.join(moved[:3])}" if moved else "")
                             + "; apply the commit's change there by hand)")
        return
    ours = target.read_text()
    if renames and code:
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


TOKEN = re.compile(r"\w+|[^\w\s]")
NAME = re.compile(r"[A-Za-z_]\w*")


def names_only_differ(a: str, b: str) -> bool:
    """True when a and b have the same tokens apart from identifiers and
    whitespace: what symbol, field and type renames change."""
    ta, tb = TOKEN.findall(a), TOKEN.findall(b)
    return len(ta) == len(tb) and all(x == y or (NAME.fullmatch(x) and NAME.fullmatch(y)) for x, y in zip(ta, tb))


def find_copies(path: str, text: str) -> list[str]:
    """Files of the same kind under src/ and include/ that equal text apart
    from renamed names: where this branch moved a file the commit edits."""
    suffix = Path(path).suffix
    found = []
    for top in ("src", "include"):
        for p in sorted((ROOT / top).rglob("*" + suffix)):
            if names_only_differ(text, p.read_text()):
                found.append(p.relative_to(ROOT).as_posix())
    return found


CONFLICTS: list[str] = []
CONFLICT_BLOCK = re.compile(r"^<<<<<<< current\n(.*?)^=======\n(.*?)^>>>>>>> commit\n", re.M | re.S)
MERGED_HEADERS: dict[str, header_merge.Report] = {}
COMMIT = ""


def merge_header(path: str, ours: str, theirs: str, base: str) -> bool:
    """Merge a conflicting header by meaning (tools/header_merge.py). False
    if it cannot be read; the conflict markers of git merge-file then stay."""
    try:
        text, rep = header_merge.merge(ours, theirs, base, *header_types(path, ours, theirs),
                                       probe=HeaderProbe(path))
    except header_merge.MergeError as e:
        print(f"{path}: cannot merge by meaning: {e}")
        return False
    (ROOT / path).write_text(text)
    print(rep.summary(path))
    MERGED_HEADERS[path] = rep
    for c in rep.conflicts:
        CONFLICTS.append(f"{path} ({c})")
    return True


def header_types(path: str, ours: str, theirs: str) -> tuple[header_merge.Types, header_merge.Types]:
    """Type sizes to lay out struct fields: this branch's, and the commit's.
    Each has its own side's definitions first, since a struct both sides
    declare can have a different size on each."""
    mine = [p.read_text() for p in sorted((ROOT / "include").rglob("*.h")) if p != ROOT / path]
    commit = [show(COMMIT, name) or "" for name in
              git("diff-tree", "--no-commit-id", "-r", "--name-only", f"{COMMIT}^", COMMIT).splitlines()
              if name.startswith("include/") and name != path]
    return (header_merge.Types([ours] + mine + [theirs] + commit),
            header_merge.Types([theirs] + commit + [ours] + mine))


class HeaderProbe:
    """Compiles a version of the header at path in its side's tree, for
    header_merge.merge (b31: three ARM7 merges placed fields at offsets
    the compiler did not use). The trees are HEAD's include/ for 'ours',
    the commit's for 'theirs', its parent's for 'base', and for 'merged'
    the include/ of this checkout, where a header still in conflict counts
    as HEAD's and one the commit changed that this branch did not as the
    commit's. It compiles with the pinned mwccarm of the checkout holding
    this script, with the flags of each module that includes the header."""

    def __init__(self, path: str):
        self.rel = path.removeprefix("include/")
        self.dir = ROOT / "build" / "apply_commit" / "probe"
        self.flag_sets: list[str] | None = None

    def __call__(self, side: str, text: str) -> header_merge.Compiled:
        import configure
        cc = configure.ROOT / configure.CC
        if not cc.exists():
            raise header_merge.MergeError(f"{configure.CC} is missing (run make setup); without it the"
                                          " merged struct layout cannot be checked")
        include = self.tree(side)
        results = []
        for flags in self.flags(configure):
            results.append(header_merge.compile_header(cc, flags, include, self.rel, text))
        if any(r != results[0] for r in results):
            raise header_merge.MergeError(f"{self.rel} lays out differently with the ARM9 and ARM7 flags")
        return results[0]

    def tree(self, side: str) -> Path:
        import io
        import shutil
        import tarfile
        root = self.dir / side
        if root.exists():
            shutil.rmtree(root)
        root.mkdir(parents=True)
        rev = {"ours": "HEAD", "theirs": COMMIT, "base": f"{COMMIT}^"}.get(side)
        if rev is not None:
            data = subprocess.run(["git", "archive", rev, "include"], cwd=ROOT, capture_output=True,
                                  check=True).stdout
            with tarfile.open(fileobj=io.BytesIO(data)) as tar:
                tar.extractall(root, filter="data")
            return root / "include"
        shutil.copytree(ROOT / "include", root / "include")
        changed = git("diff-tree", "--no-commit-id", "-r", "--name-only", f"{COMMIT}^", COMMIT).splitlines()
        for p in (root / "include").rglob("*.h"):
            name = p.relative_to(root).as_posix()
            text = p.read_text()
            if "<<<<<<< current" in text:
                p.write_text(show("HEAD", name) or "")
            elif name in changed and text == (show(f"{COMMIT}^", name) or ""):
                p.write_text(show(COMMIT, name) or "")
        return root / "include"

    def flags(self, configure) -> list[str]:
        """ARM7's flags if a file under src/arm7/ includes the header, ARM9's if
        another file does (directly or through other headers); both if both or
        neither."""
        if self.flag_sets is not None:
            return self.flag_sets
        users, todo = set(), [self.rel]
        files = list((ROOT / "include").rglob("*.h")) + list((ROOT / "src").rglob("*.c"))
        includes = {p: set(INCLUDE.findall(p.read_text(errors="replace"))) for p in files}
        while todo:
            name = todo.pop()
            for p, names in includes.items():
                if name in names and p not in users:
                    users.add(p)
                    if p.suffix == ".h":
                        todo.append(p.relative_to(ROOT / "include").as_posix())
        arm7 = any(p.suffix == ".c" and configure.is_arm7_source(p.relative_to(ROOT)) for p in users)
        arm9 = any(p.suffix == ".c" and not configure.is_arm7_source(p.relative_to(ROOT)) for p in users)
        self.flag_sets = [configure.CFLAGS7 if arm7 else configure.CFLAGS] if arm7 != arm9 \
            else [configure.CFLAGS, configure.CFLAGS7]
        return self.flag_sets


def member_access(name: str) -> re.Pattern:
    return re.compile(rf"(->|\.)(\s*){re.escape(name)}(?!\w)")


def apply_header_renames(paths: list[str]) -> None:
    """After a header merge kept mainline's field and type names, rewrite
    the commit's sources to use them. Fields match as member accesses
    (->name, .name) without knowing the struct behind them, so a field is
    renamed only when no struct in include/ still has a field of its old
    name. Otherwise `p->mapWidth` of another struct would be renamed too;
    the replay stops and lists the uses for the agent to fix."""
    fields, types = {}, {}
    for rep in MERGED_HEADERS.values():
        for old, new in rep.renames.items():
            if fields.setdefault(old, (new, rep.rename_structs.get(old, "?")))[0] != new:
                print(f"warning: {old} maps to both {fields[old][0]} and {new}; renamed to {fields[old][0]}")
        types.update(rep.type_renames)
    texts = [p.read_text() for p in sorted((ROOT / "include").rglob("*.h"))]
    owners = header_merge.struct_fields(texts)
    safe = {}
    for old, (new, where) in fields.items():
        others = sorted(n for n, names in owners.items() if old in names)
        if not others:
            safe[old] = new
            continue
        uses = []
        for path in paths:
            if path.endswith((".c", ".h")) and (ROOT / path).exists():
                text = (ROOT / path).read_text()
                uses += [f"{path}:{text.count(chr(10), 0, m.start()) + 1}" for m in member_access(old).finditer(text)]
        if uses:
            CONFLICTS.append(f"{where}.{old} is {new} on mainline, but {', '.join(others)} also"
                             f" {'has' if len(others) == 1 else 'have'} a field {old}, so its uses were not"
                             f" renamed; change the ones that mean {where} to ->{new}"
                             f" ({', '.join(dict.fromkeys(uses))})")
    for path in paths:
        if path.endswith(".c") or (path.endswith(".h") and path not in MERGED_HEADERS):
            rewrite(path, safe, types)


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


def rename_in_sources(renames, skip) -> None:
    """Apply the commit's renames to the sources it did not change, all in
    one pass so that A -> B and C -> A do not chain into C -> B. The files
    in skip came from the commit (merge_file renamed this branch's side
    before merging), so they have the new names already."""
    names = dict(renames)
    files = list((ROOT / "src").rglob("*.[ch]")) + list((ROOT / "include").rglob("*.h"))
    for p in files:
        if p.relative_to(ROOT).as_posix() in skip:
            continue
        text = p.read_text()
        new = header_merge.rename_words(text, names)
        if new != text:
            p.write_text(new)


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
        if old is None and not (ROOT / path).exists():
            # A file this branch does not have yet, such as a new module's
            # config: nothing to merge it with.
            merge_file(path, old, new, dict(renames))
        elif path.startswith("config/") and name == "delinks.txt":
            apply_delinks(path, old, new)
        elif path.startswith("config/") and name == "symbols.txt":
            renames += apply_symbols(path, old, new)
        elif path.startswith("config/") and name == "relocs.txt":
            apply_relocs(path, old, new)
        else:
            merge_file(path, old, new, dict(renames))
    paths = [ln.split("\t", 1)[1] for ln in changes]
    rename_in_sources(renames, set(paths))
    for old, new in renames:
        print(f"renamed {old} -> {new}")
    apply_header_renames(paths)
    fix_stale_names(paths)
    drop_header_prototypes(paths)
    resolve_equal_sides()
    if CONFLICTS:
        sys.exit("error: conflict in " + ", ".join(CONFLICTS) + "; resolve, then commit by hand")

    # A merged header or a commit made before `make format` existed would
    # fail make release's format check.
    sources = [p for p in paths if p.startswith(("src/", "include/")) and (ROOT / p).exists()]
    if sources:
        subprocess.run([sys.executable, Path(__file__).resolve().parent / "format.py", *sources],
                       cwd=ROOT, check=True)
    git("add", "-A", "config", "src", "include", "docs")
    # Also stage the commit's other paths (tools/, hooks/, Makefile, CLAUDE.md,
    # skills): without this, a replayed tools commit landed without its files.
    other = sorted({p for ln in changes for p in ln.split("\t")[1:]
                    if p != "PROGRESS.md" and not p.startswith(("config/", "src/", "include/", "docs/"))})
    if other:
        git("add", "-A", "--", *other)
    if not git("diff", "--cached", "--name-only"):
        print("error: nothing left to commit: this branch already has the change, or the merge"
              " kept this branch's version (see the notes above)", file=sys.stderr)
        return 1
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
