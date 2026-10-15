#!/usr/bin/env python3
"""Declare each function, variable and shared macro once, in a header.

    python3 tools/decls.py check [--list]
    python3 tools/decls.py dedupe [FILE.c ...]
    python3 tools/decls.py declare HEADER [NAME ...] [--dir src/DIR]
    python3 tools/decls.py reconcile [FILE.c ...] [--name NAME] [--write]
    python3 tools/decls.py unify NAME [--variant PROTO] [--apply]
    python3 tools/decls.py macro HEADER NEW OLD [OLD ...] [--comment TEXT]

docs/readability.md gives the rules this serves; every command leaves the
code alone, which `tools/objcmp.py compare` confirms after a build.

check    Lists each file-scope declaration in a C file of a name that a
         header in include/ declares, and each name two headers declare.
         `make release` fails on any not in config/AWRE/local_decls.txt
         (one `path name reason` line each).
dedupe   Removes those declarations from the C files (all of src/ when no
         FILE is given; only NAMEs given with --name) and includes the header that declares the name when
         the file does not already include it, directly or through another
         header. A declaration with another type than the header's stays
         and is listed: removing it may change the caller's code, so it
         needs a change of its own (issue 0062).
declare  Writes a declaration of each NAME, and of each function defined in
         a --dir, into HEADER (created when missing): a header's declaration
         as it is, else a function's prototype from its definition in src/
         and a variable's type from its definition.
         Other headers lose their declaration of NAME; dedupe runs for these
         names; then every file that uses a NAME and no longer sees a
         declaration of it includes HEADER.
reconcile
         Gives each local declaration that differs from its header the
         header's text, one per file per round: builds with ninja -k 0, puts
         back the old text where the file does not compile or where
         `objcmp.py compare` finds a function of the file changed, and goes
         on. --write lists the ones put back in local_decls.txt, with why.
         Needs `objcmp.py snap` of the build before. Run dedupe afterwards.
unify    Tries each prototype that src/ and include/ use for NAME (or only
         --variant) as the one prototype of its definition, the header and
         every C file, keeping each site's parameter names; builds and
         prints how many files fail and which functions it fixes or breaks
         against the current build. --apply keeps the first that breaks
         nothing and fixes something (or leaves no difference). The callers' code depends on
         the widths a prototype gives, so the one the original was built
         with changes nothing.
macro    Defines NEW in HEADER with the body that OLD has where a C file or
         header defines it (all must agree, after expanding other macros of
         that file), removes those definitions, renames OLD to NEW in every
         file where OLD means that body, and includes HEADER there.
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from merge_files import Item, add_include, decl_key, norm, prototype, sig_types, split_items  # noqa: E402

INCLUDE = ROOT / "include"
SEARCH = [INCLUDE, ROOT / "build/gen", ROOT / "extracted/AWRE"]
EXCEPTIONS = ROOT / "config/AWRE/local_decls.txt"
INC_RE = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)


def is_decl(it: Item) -> bool:
    """A declaration, not a definition: a prototype or an extern variable."""
    if it.kind != "decl" or re.match(r"\s*(typedef|static|struct|union|enum)\b", it.body):
        return False
    return bool(it.name) or it.body.lstrip().startswith("extern")


def type_names(text: str) -> set:
    """Typedef names and struct, union and enum tags a file defines."""
    out = set()
    for it in split_items(text):
        if it.kind == "decl" and re.match(r"\s*(typedef|struct|union|enum)\b", it.body):
            m = re.match(r"\s*(?:typedef\s+)?(?:struct|union|enum)\s+(\w+)\s*\{", it.body)
            if m:
                out.add(m.group(1))
            if it.body.lstrip().startswith("typedef"):
                m = re.search(r"(\w+)\s*(\[[^\]]*\])*\s*;\s*$", it.body) or \
                    re.search(r"\(\s*\*\s*(\w+)\s*\)", it.body)
                if m:
                    out.add(m.group(1))
    return out


def rel(p: Path) -> str:
    return p.relative_to(ROOT).as_posix()


class Tree:
    """Headers in include/, what they declare and include."""

    def __init__(self):
        self.text = {}
        self.decls = {}    # name -> [header rel to include/]
        self.items = {}    # (header, name) -> Item
        self.macros = {}   # header -> {name: Item}
        self.types = {}    # header -> set of typedef names and struct tags
        for h in sorted(INCLUDE.rglob("*.h")):
            key = h.relative_to(INCLUDE).as_posix()
            self.text[key] = h.read_text(errors="replace")
            self.macros[key] = {}
            self.types[key] = type_names(self.text[key])
            for it in split_items(self.text[key]):
                if is_decl(it):
                    name = decl_key(it)
                    self.decls.setdefault(name, []).append(key)
                    self.items[(key, name)] = it
                elif it.kind == "define":
                    self.macros[key][it.name] = it
        self._closure = {}

    def includes(self, text: str) -> list:
        return [m.group(1) for m in INC_RE.finditer(text)]

    def closure(self, text: str) -> set:
        """Headers in include/ that text includes, directly or not."""
        seen, todo = set(), self.includes(text)
        while todo:
            h = todo.pop()
            if h in seen or h not in self.text:
                continue
            seen.add(h)
            if h not in self._closure:
                self._closure[h] = self.includes(self.text[h])
            todo += self._closure[h]
        return seen


def c_files(paths) -> list:
    if paths:
        return [ROOT / p for p in paths]
    return sorted((ROOT / "src").rglob("*.c"))


def remove_line(text: str, it: Item) -> str:
    """Remove a declaration with the rest of its line; keep comments above."""
    pat = r"(?m)^[ \t]*" + re.escape(it.body) + r"[ \t]*(/\*[^\n]*?\*/|//[^\n]*)?[ \t]*\n"
    return re.sub(pat, "", text, count=1)


def same_type(a: Item, b: Item) -> bool:
    """Two extern declarations of one variable that differ only in an array
    length (`T x[]` and `T x[N]`) or in `extern`."""
    def t(it):
        return re.sub(r"\[[^\]]*\]", "[]", re.sub(r"^extern\s+", "", it.body.strip()))
    return not a.name and not b.name and norm(t(a)) == norm(t(b))


def exceptions() -> set:
    out = set()
    if EXCEPTIONS.exists():
        for line in EXCEPTIONS.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                path, name = line.split()[:2]
                out.add((path, name))
    return out


def copies(tree: Tree, files: list):
    """(file, item, header) for each local copy of a header declaration."""
    for p in files:
        for it in split_items(p.read_text(errors="replace")):
            if is_decl(it):
                name = decl_key(it)
                if name in tree.decls:
                    yield p, it, tree.decls[name][0]


def check_errors() -> list:
    """What `check` reports, one line each (used by decomp_dev_report.py)."""
    tree = Tree()
    allowed = exceptions()
    found = [(rel(p), decl_key(it), h) for p, it, h in copies(tree, c_files(None))]
    errors = [f"{path}: {name} is declared in include/{h}" for path, name, h in found
              if (path, name) not in allowed]
    for n, hs in sorted(tree.decls.items()):
        if len(set(hs)) > 1:
            errors.append(f"{n}: declared in {' and '.join('include/' + h for h in sorted(set(hs)))}")
    for path, name in sorted(allowed - {f[:2] for f in found}):
        errors.append(f"{rel(EXCEPTIONS)}: {path} no longer declares {name}; remove the line")
    return errors


def cmd_check(args) -> int:
    errors = check_errors()
    if args.list:
        print("\n".join(errors))
    print(f"decls: {len(errors)} declarations that break the rules (docs/readability.md)")
    return 1 if errors else 0


def dedupe(tree: Tree, files: list, names=None) -> list:
    differ = []
    allowed = exceptions()
    for p in files:
        text = p.read_text(errors="replace")
        have = tree.closure(text)
        remove, kept = [], set()
        for it in split_items(text):
            if not is_decl(it):
                continue
            name = decl_key(it)
            if name not in tree.decls:
                continue
            h = tree.decls[name][0]
            ref = tree.items[(h, name)]
            # Names outside the selection still count for what the file keeps.
            chosen = names is None or name in names
            if (rel(p), name) in allowed:
                kept.add(name)
            elif sig_types(ref.body) != sig_types(it.body) and not same_type(ref, it):
                # Another type: the caller's code may depend on it. Left for
                # a change of its own (protocheck.py, issue 0062).
                if chosen:
                    differ.append(f"{rel(p)}: {it.body.strip()}  (include/{h}: {ref.body.strip()})")
                kept.add(name)
            elif chosen:
                remove.append((it, h))
        for it in split_items(text):
            # A function the file defines counts like a declaration it keeps.
            if it.kind == "func" and it.name in tree.decls:
                ref = tree.items[(tree.decls[it.name][0], it.name)]
                if sig_types(ref.body) != sig_types(prototype(it)):
                    kept.add(it.name)
        # A header the file does not include yet must not bring in another
        # declaration of a name the file keeps with its own type, or another
        # definition of one of its macros or types.
        mine = {it.name: norm(it.body) for it in split_items(text) if it.kind == "define"}
        my_types = type_names(text)
        new, added = text, []
        for it, h in remove:
            if h not in have:
                brings = tree.closure(f'#include "{h}"')
                if any(set(tree.decls[k]) & brings for k in kept):
                    continue
                if any(n in mine and norm(tree.macros[b][n].body) != mine[n]
                       for b in brings for n in tree.macros.get(b, {})):
                    continue
                if any(my_types & tree.types.get(b, set()) for b in brings):
                    continue
                added.append(h)
                have |= brings
            new = remove_line(new, it)
        for h in added:
            new = add_include(new, f'#include "{h}"')
        if new != text:
            p.write_text(new)
    return differ


def cmd_dedupe(args) -> int:
    differ = dedupe(Tree(), c_files(args.files), set(args.name) if args.name else None)
    for d in differ:
        print(f"differs: {d}")
    return 0


def definition(name: str):
    """A function's prototype or a variable's extern declaration, from its
    definition in src/."""
    for p in sorted((ROOT / "src").rglob("*.c")):
        text = p.read_text(errors="replace")
        if name not in text:
            continue
        for it in split_items(text):
            if it.kind == "func" and it.name == name and not it.body.lstrip().startswith("static"):
                return prototype(it)
            if it.kind == "decl" and not it.name and decl_key(it) == name \
                    and not re.match(r"\s*(extern|static|typedef)\b", it.body):
                body = re.sub(r"\s*=.*", "", it.body.strip().rstrip(";"), flags=re.S)
                return "extern " + re.sub(r"\[[^\]]*\]", "[]", body) + ";"
    return None


def strip_comments(text: str) -> str:
    return re.sub(r"/\*.*?\*/|//[^\n]*", " ", text, flags=re.S)


def dir_functions(directory: Path) -> list:
    """Non-static functions defined in the C files of a directory (not below it)."""
    names = []
    for p in sorted(directory.glob("*.c")):
        for it in split_items(p.read_text(errors="replace")):
            if it.kind == "func" and it.name and not re.match(r"\s*static\b", it.body):
                names.append(it.name)
    return names


def new_header(header: str, decls: list) -> str:
    from merge_files import typedef_headers
    guard = re.sub(r"\W", "_", header.upper())
    tdh = typedef_headers()
    needed = sorted({tdh[w] for d in decls for w in re.findall(r"[A-Za-z_]\w*", d)
                     if w in tdh and tdh[w] not in ("types.h", header)})
    lines = [f"#ifndef {guard}", f"#define {guard}", "", '#include "types.h"']
    lines += [f'#include "{h}"' for h in needed] + ["", "#endif", ""]
    return "\n".join(lines)


def conflict(tree: Tree, text: str, brings: set, extra: dict) -> str:
    """Why including the headers in brings (and the declarations in extra,
    name -> Item, that one of them gets) would break this file, or ""."""
    items = split_items(text)
    for it in items:
        if it.kind == "func" and it.name:
            it = split_items(prototype(it))[0]
        elif not is_decl(it):
            continue
        n = decl_key(it)
        refs = [tree.items[(h, n)] for h in tree.decls.get(n, []) if h in brings]
        if n in extra:
            refs.append(extra[n])
        for ref in refs:
            if sig_types(ref.body) != sig_types(it.body) and not same_type(ref, it):
                return f"declares {it.body.strip()}"
    mine = {it.name: norm(it.body) for it in items if it.kind == "define"}
    for h in brings:
        for n, m in tree.macros.get(h, {}).items():
            if n in mine and norm(m.body) != mine[n]:
                return f"defines macro {n} otherwise"
    my_types = type_names(text)
    for h in brings:
        if my_types & tree.types.get(h, set()):
            return f"defines type {' '.join(sorted(my_types & tree.types[h]))}"
    return ""


def cmd_declare(args) -> int:
    return declare(args.header, list(args.names), args.dir or [])


def declare(header: str, names: list, dirs=()) -> int:
    tree = Tree()
    hpath = INCLUDE / header
    for d in dirs:
        # A function a header already declares has a home.
        names += [n for n in dir_functions(ROOT / d) if n not in names and n not in tree.decls]
    from merge_files import typedef_headers
    known = set(typedef_headers()) | {"BOOL", "TRUE", "FALSE", "NULL"}
    decls, notes, already = {}, [], set()
    for name in names:
        if header in tree.decls.get(name, []):
            # Declared here already: other headers still lose theirs.
            decls[name] = tree.items[(header, name)]
            already.add(name)
            continue
        # A header's declaration moves as it is: changing a type is a change
        # of its own, which may change the callers' code.
        hs = tree.decls.get(name)
        decl = tree.items[(hs[0], name)].body.strip() if hs else definition(name)
        if decl is None:
            sys.exit(f"error: {name} has no definition in src/ and no declaration in include/")
        # A type that only a C file defines cannot be named in a header yet.
        types = re.findall(r"\b([A-Z]\w*)\s*\*?\s*\w*\s*[,)\[;]?", re.sub(r"\b" + name + r"\b", "", decl))
        missing = sorted({t for t in types if t not in known and not t.isupper()})
        if missing:
            notes.append(f"kept: {name}: {', '.join(missing)} is defined in no header")
            continue
        decls[name] = split_items(decl)[0]

    # Every C file and header, with what it includes.
    texts = {p: p.read_text(errors="replace") for p in c_files(None)}
    texts.update({INCLUDE / h: t for h, t in tree.text.items()})

    def sees(p):
        own = {p.relative_to(INCLUDE).as_posix()} if p.is_relative_to(INCLUDE) else set()
        return tree.closure(texts[p]) | own

    seen = {p: sees(p) for p in texts}
    c_paths = [p for p in texts if p.suffix == ".c"]
    hbrings = tree.closure(f'#include "{header}"') | {header} if hpath.exists() else set()
    brings_new = (hbrings or set()) | {header}

    plan = {}   # path -> set of names it needs the header for
    for name, item in list(decls.items()):
        word = re.compile(rf"\b{re.escape(name)}\b")
        # Files that see the header already get the new declaration.
        bad = [p for p in c_paths if header in seen[p] and conflict(tree, texts[p], set(), {name: item})]
        need = {}
        for x in sorted(set(tree.decls.get(name, [])) - {header}):
            for p in texts:
                if x not in seen[p] or header in seen[p]:
                    continue
                code = strip_comments(texts[p])
                local = any(is_decl(it) and decl_key(it) == name for it in split_items(texts[p]))
                if word.search(code) and not local and p != INCLUDE / x:
                    need[p] = x
            # The old header itself, when it uses the name beyond declaring it.
            if word.search(strip_comments(remove_line(tree.text[x], tree.items[(x, name)]))):
                need[INCLUDE / x] = x
        # Who would get the header through each file that needs it.
        for p in need:
            reach = [p] if p.suffix == ".c" else \
                [q for q in c_paths if p.relative_to(INCLUDE).as_posix() in seen[q] and header not in seen[q]]
            for q in reach:
                why = conflict(tree, texts[q], brings_new, decls)
                if why:
                    bad.append(q)
                    break
        if bad:
            notes.append(f"kept: {name}: {rel(bad[0])} {conflict(tree, texts[bad[0]], brings_new, decls) or 'declares it otherwise'}")
            del decls[name]
            continue
        for p in need:
            plan.setdefault(p, set()).add(name)

    if not decls:
        print("\n".join(notes))
        print("declared nothing")
        return 1
    if not hpath.exists():
        hpath.parent.mkdir(parents=True, exist_ok=True)
        hpath.write_text(new_header(header, [it.body for it in decls.values()]))
    text = hpath.read_text()
    m = list(re.finditer(r"^#endif", text, re.M))
    pos = m[-1].start() if m else len(text)
    new = [it.body.strip() for n, it in decls.items() if n not in already]
    if new:
        text = text[:pos].rstrip("\n") + "\n\n" + "\n".join(new) + "\n\n" + text[pos:]
        hpath.write_text(text)
    # Other headers lose their declaration.
    for name in decls:
        for h in sorted(set(tree.decls.get(name, [])) - {header}):
            p = INCLUDE / h
            p.write_text(remove_line(p.read_text(), tree.items[(h, name)]))
    for p in plan:
        t = p.read_text()
        if f'#include "{header}"' not in t:
            p.write_text(add_include(t, f'#include "{header}"'))
    differ = dedupe(Tree(), c_files(None), set(decls))
    for d in differ:
        print(f"differs: {d}")
    for d in notes:
        print(d)
    print(f"declared {len(decls) - len(already)} names in include/{header}, {len(already)} there "
          f"already; {len(plan)} files include it now")
    return 0


def build_failures() -> set:
    """Build everything that can be built; the C files that do not compile."""
    import subprocess
    subprocess.run([sys.executable, ROOT / "tools/configure.py"], cwd=ROOT, check=True,
                   stdout=subprocess.DEVNULL)
    (ROOT / "build").mkdir(exist_ok=True)
    out = subprocess.run(["flock", "build/.ninja.lock", "ninja", "-k", "0"], cwd=ROOT,
                         capture_output=True, text=True).stdout
    bad = set()
    for m in re.finditer(r"^FAILED: (?:\[code=\d+\] )?(build/(?:arm7/)?(src/\S+)\.o)\b", out, re.M):
        bad.add(ROOT / (m.group(2) + ".c"))
    return bad


def cmd_reconcile(args) -> int:
    """Give each local declaration that differs from its header the header's
    text, where the file still compiles and no function's code changes."""
    import json
    from objcmp import DEFAULT, changed
    base = json.loads(DEFAULT.read_text())
    tree = Tree()
    allowed = exceptions()
    todo = {}   # path -> [(old body, new body, name)]
    for p, it, h in copies(tree, c_files(args.files)):
        name = decl_key(it)
        if args.name and name not in args.name or (rel(p), name) in allowed:
            continue
        ref = tree.items[(h, name)]
        if sig_types(ref.body) != sig_types(it.body) and not same_type(ref, it):
            todo.setdefault(p, []).append((it.body, ref.body.strip(), name))
    originals = {p: p.read_text() for p in todo}
    kept_old = {}   # path -> [(old, new, name, why)]
    rnd = 0
    # Each round tries the next declaration of every file that is not done.
    while todo:
        rnd += 1
        trying = {}
        for p, changes in todo.items():
            old, new, name = changes[0]
            text = p.read_text()
            p.write_text(text.replace(old, new, 1))
            trying[p] = (old, new, name)
        while True:
            bad = build_failures() & set(trying)
            for p in bad:
                old, new, name = trying.pop(p)
                p.write_text(p.read_text().replace(new, old, 1))
                kept_old.setdefault(p, []).append((old, new, name, "does not compile with the header's type"))
            if not bad:
                break
        owners = {}
        moved = changed(base, owners)
        srcs = {ROOT / (re.sub(r"^build/(arm7/)?", "", o)[:-2] + ".c") for n in moved for o in owners.get(n, [])}
        for p in srcs & set(trying):
            old, new, name = trying.pop(p)
            p.write_text(p.read_text().replace(new, old, 1))
            kept_old.setdefault(p, []).append((old, new, name, "needed to match"))
        if srcs - set(originals):
            sys.exit(f"error: code changed outside the files being reconciled: {sorted(map(rel, srcs - set(originals)))}")
        print(f"round {rnd}: {len(trying)} declarations now as in their header, "
              f"{sum(len(v) for v in kept_old.values())} kept so far")
        for p in list(todo):
            todo[p].pop(0)
            if not todo[p]:
                del todo[p]
    if build_failures():
        sys.exit("error: the build fails after reconcile")
    if changed(base):
        sys.exit("error: code changed after reconcile")
    lines = [f"{rel(p)} {name} {why}" for p, v in sorted(kept_old.items()) for _, _, name, why in v]
    if lines and args.write:
        head = "" if EXCEPTIONS.exists() else (
            "# File-scope declarations that stay in a C file although a header declares the\n"
            "# name: `path name reason`. tools/decls.py check fails on any other\n"
            "# (docs/readability.md).\n")
        old = EXCEPTIONS.read_text() if EXCEPTIONS.exists() else ""
        EXCEPTIONS.write_text(head + old + "\n".join(lines) + "\n")
    for line in lines:
        print(f"kept: {line}")
    return 0


def split_params(proto: str):
    """(return and name part, [(type, name)]) of a prototype or definition head."""
    m = re.match(r"(.*?)\((.*)\)\s*;?\s*$", proto.strip(), re.S)
    head, inner = m.group(1).strip(), m.group(2).strip()
    params = []
    if inner and inner != "void":
        for p in inner.split(","):
            words = re.findall(r"[A-Za-z_]\w*|\*|\[\]", p)
            if len(words) > 1 and re.fullmatch(r"[A-Za-z_]\w*", words[-1]) and \
                    words[-1] not in ("void", "int", "char", "short", "long", "unsigned", "signed"):
                params.append((p[:p.rindex(words[-1])].strip(), words[-1]))
            else:
                params.append((p.strip(), ""))
    return head, params


def with_types(site: str, variant: str, name: str) -> str:
    """site's prototype or definition head with variant's types, keeping
    site's parameter names and storage words (extern)."""
    shead, sparams = split_params(site)
    vhead, vparams = split_params(variant)
    if len(sparams) != len(vparams):
        names = [""] * len(vparams)
    else:
        names = [n for _, n in sparams]
    ret = re.sub(rf"\b{re.escape(name)}\s*$", "", vhead).strip()
    ret = re.sub(r"^extern\s+", "", ret)
    prefix = "extern " if shead.startswith("extern") else ""
    args = ", ".join(f"{t} {n}".strip() for (t, _), n in zip(vparams, names)) or "void"
    star = "" if ret.endswith("*") else " "
    return f"{prefix}{ret}{star}{name}({args})"


def unify_sites(name: str):
    """[(path, Item, is_definition)] of every declaration and the definition."""
    sites = []
    for p in sorted((ROOT / "src").rglob("*.c")) + sorted(INCLUDE.rglob("*.h")):
        text = p.read_text(errors="replace")
        if name not in text:
            continue
        for it in split_items(text):
            if it.kind == "func" and it.name == name:
                sites.append((p, it, True))
            elif is_decl(it) and decl_key(it) == name and it.name:
                sites.append((p, it, False))
    return sites


def apply_variant(sites, variant: str, name: str) -> dict:
    """Rewrite every site to variant's types; returns the old texts."""
    old = {}
    for p, it, is_def in sites:
        text = old.setdefault(p, p.read_text()) if p not in old else p.read_text()
        if is_def:
            head = it.body[:it.body.index("{")]
            new = with_types(head, variant, name) + " "
            text = text.replace(head, new, 1)
        else:
            text = text.replace(it.body.strip(), with_types(it.body, variant, name) + ";", 1)
        p.write_text(text)
    return old


def cmd_unify(args) -> int:
    """Try each prototype in use for NAME as the one prototype everywhere."""
    import json
    from objcmp import DEFAULT, changed
    base = json.loads(DEFAULT.read_text())
    name = args.name
    sites = unify_sites(name)
    if not any(d for _, _, d in sites):
        sys.exit(f"error: no definition of {name} in src/")
    variants = []
    for _, it, is_def in sites:
        proto = prototype(it) if is_def else it.body.strip()
        key = sig_types(re.sub(r"^extern\s+", "", proto))
        if key not in [sig_types(v) for v in variants]:
            variants.append(re.sub(r"^extern\s+", "", proto))
    if args.variant:
        variants = [args.variant]
    # Functions that differ already, for a tree in the middle of a change.
    before = set(changed(base)) if not build_failures() else set()
    results = []
    for v in variants:
        old = apply_variant(sites, v, name)
        bad = build_failures()
        moved = set(changed(base)) if not bad else set()
        fixed, broke = sorted(before - moved), sorted(moved - before)
        results.append((len(bad) + len(moved), v))
        print(f"{v}: {len(bad)} files fail to compile"
              + (f" ({', '.join(sorted(map(rel, bad))[:4])})" if bad else "")
              + f", {len(moved)} functions differ"
              + (f"; fixes {', '.join(fixed)}" if fixed else "")
              + (f"; breaks {', '.join(broke)}" if broke else ""))
        if args.apply and not bad and not broke and (fixed or not moved):
            print(f"applied: {v}")
            return 0
        for p, t in old.items():
            p.write_text(t)
    build_failures()  # the objects of the tree as it was
    best = min(results)
    if args.apply:
        print(f"nothing applied: every prototype changes code or fails; best: {best[1]}")
        return 1
    return 0


def expand(body: str, defines: dict, depth: int = 0) -> str:
    """Object-like macros of the file expanded in body, to compare bodies."""
    if depth > 8:
        return body
    def sub(m):
        d = defines.get(m.group(0))
        return f"({expand(d, defines, depth + 1)})" if d is not None else m.group(0)
    return re.sub(r"\b[A-Z_][A-Z0-9_]*\b", sub, body)


def macro_body(it: Item) -> tuple:
    m = re.match(r"#\s*define\s+(\w+)(\([^)]*\))?\s*(.*)", it.body.replace("\\\n", " "), re.S)
    return m.group(2) or "", re.sub(r"\s+", "", m.group(3))


def cmd_macro(args) -> int:
    tree = Tree()
    olds = sorted(set(args.old))
    header = args.header
    files = [(rel(p), p) for p in sorted((ROOT / "src").rglob("*.c"))] + \
        [("include/" + h, INCLUDE / h) for h in tree.text]
    # The body of each definition of an OLD name, with the file's other
    # object-like macros expanded in it.
    defs = {}      # (file, name) -> (Item, body)
    bodies = {}    # body -> [(file, name)]
    for key, p in files:
        text = p.read_text(errors="replace")
        if not any(re.search(rf"\b{o}\b", text) for o in olds):
            continue
        # Object-like macros of the headers it includes count too.
        inc = tree.closure(text) if key.startswith("src/") else \
            tree.closure(f'#include "{key[len("include/"):]}"')
        known = {}
        for h in inc:
            for n, m in tree.macros.get(h, {}).items():
                params, body = macro_body(m)
                if not params and n not in olds:
                    known[n] = body
        for it in split_items(text):
            if it.kind != "define":
                continue
            params, body = macro_body(it)
            if it.name in olds:
                full = params + expand(body, {k: v for k, v in known.items() if k != it.name})
                # Brackets an expansion adds do not change the meaning; a
                # rename that changes code shows in objcmp.py.
                full = params + re.sub(r"[()]", "", full[len(params):])
                defs[(key, it.name)] = (it, full)
                bodies.setdefault(full, []).append((key, it.name))
            if not params:
                known[it.name] = body
    if not bodies:
        sys.exit(f"error: no definition of {' or '.join(olds)}")
    want = args.body or max(bodies, key=lambda b: len(bodies[b]))
    if want not in bodies:
        sys.exit(f"error: no definition has the body {want}")
    # NEW may exist already only as this body (a second run that renames
    # more files to it).
    for h, ms in tree.macros.items():
        if args.new in ms:
            params, body = macro_body(ms[args.new])
            known = {n: macro_body(m)[1] for n, m in tree.macros[h].items() if not macro_body(m)[0]}
            have = params + re.sub(r"[()]", "", expand(body, known))
            if have != want:
                sys.exit(f"error: {args.new} is already a macro in include/{h} with another body")
    for b, ks in sorted(bodies.items()):
        if b != want:
            names = sorted({k for _, k in ks})
            print(f"kept: {len(ks)} definitions of {'/'.join(names)} as {b}: "
                  f"{' '.join(f for f, _ in ks[:4])}{' ...' if len(ks) > 4 else ''}")
    sample = defs[bodies[want][0]][0]
    params, _ = macro_body(sample)
    raw = re.match(r"#\s*define\s+\w+(\([^)]*\))?\s*(.*)", sample.body, re.S).group(2)
    raw = re.sub(r"\s*\\\n\s*", " ", raw).strip()

    def meaning(key: str, text: str, name: str):
        """The body NAME has in this file: its own definition, or the one
        of the headers it includes."""
        if (key, name) in defs:
            return defs[(key, name)][1]
        inc = tree.closure(text) if key.startswith("src/") else \
            tree.closure(f'#include "{key[len("include/"):]}"') - {key[len("include/"):]}
        found = {defs[("include/" + h, name)][1] for h in inc if ("include/" + h, name) in defs}
        return found.pop() if len(found) == 1 else None

    changed = 0
    for key, p in files:
        if key == "include/" + header:
            continue
        text = p.read_text(errors="replace")
        names = [o for o in olds if re.search(rf"\b{o}\b", text) and meaning(key, text, o) == want]
        if not names:
            continue
        for o in names:
            if (key, o) in defs:
                it = defs[(key, o)][0]
                text = re.sub(r"(?m)^" + re.escape(it.text) + r"[ \t]*\n", "", text, count=1)
            text = re.sub(rf"\b{o}\b", args.new, text)
        own = key[len("include/"):] if key.startswith("include/") else None
        if header not in tree.closure(text) and not (own and header in tree.closure(f'#include "{own}"')):
            text = add_include(text, f'#include "{header}"')
        p.write_text(text)
        changed += 1
    hpath = INCLUDE / header
    htext = hpath.read_text()
    if not re.search(rf"^#\s*define\s+{re.escape(args.new)}\b", htext, re.M):
        comment = f"/* {args.comment} */\n" if args.comment else ""
        m = list(re.finditer(r"^#endif", htext, re.M))
        pos = m[-1].start() if m else len(htext)
        htext = (htext[:pos].rstrip("\n") + "\n\n" + comment +
                 f"#define {args.new}{params} {raw}\n\n" + htext[pos:])
        hpath.write_text(htext)
    print(f"macro: {args.new} in include/{header}; {changed} files changed")
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = ap.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("check")
    c.add_argument("--list", action="store_true")
    d = sub.add_parser("dedupe")
    d.add_argument("files", nargs="*")
    d.add_argument("--name", action="append")
    e = sub.add_parser("declare")
    e.add_argument("header")
    e.add_argument("names", nargs="*")
    e.add_argument("--dir", action="append", help="also every function defined in this directory")
    r = sub.add_parser("reconcile")
    r.add_argument("files", nargs="*")
    r.add_argument("--name", action="append")
    r.add_argument("--write", action="store_true", help=f"list the kept ones in {EXCEPTIONS.name}")
    u = sub.add_parser("unify")
    u.add_argument("name")
    u.add_argument("--variant", help="try only this prototype")
    u.add_argument("--apply", action="store_true", help="keep the first prototype that changes nothing")
    m = sub.add_parser("macro")
    m.add_argument("header")
    m.add_argument("new")
    m.add_argument("old", nargs="+")
    m.add_argument("--body", help="the body to use, as printed by a dry run; default: the most common")
    m.add_argument("--comment")
    args = ap.parse_args()
    return {"check": cmd_check, "dedupe": cmd_dedupe, "declare": cmd_declare,
            "reconcile": cmd_reconcile, "unify": cmd_unify, "macro": cmd_macro}[args.cmd](args)


if __name__ == "__main__":
    sys.exit(main())
