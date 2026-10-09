"""Merge two versions of a C header by meaning. apply_commit.py uses it when
`git merge-file` cannot merge a header under include/.

Agents add fields to the same shared structs at the same time. A text merge
conflicts as soon as two of them split the same padding array, so this
reads both versions as declarations instead of lines:

- Struct fields are placed by byte offset, computed from their types the
  way the compiler lays them out. The result has the named fields of both
  sides, with padding written again between them. Where both sides name
  the same bytes differently, mainline's name stays and the pair is
  returned so that apply_commit.py can rewrite the commit's sources.
- Typedefs, structs, #defines, externs and prototypes that only one side
  has are kept. Where both sides changed the same one, mainline's version
  stays and the clash is reported.
- A struct the commit names differently from mainline (both sides declare
  the same extern with it) is merged into mainline's struct.
- Fields that overlap without matching are reported as conflicts.

The base (the commit's parent version, empty for a header both sides
added) tells an edit from an addition: where mainline still has the base's
version of a field or declaration, the commit's version wins, as in a
3-way merge.
"""

import re
from collections import Counter
from dataclasses import dataclass, field


class MergeError(Exception):
    """Something this merge cannot read or lay out; left to a person."""


# ---------------------------------------------------------------- reading

def split_code(line: str, in_comment: bool) -> tuple[str, bool]:
    """The code part of a line, and whether a /* comment is open after it."""
    out, i = [], 0
    while i < len(line):
        if in_comment:
            j = line.find("*/", i)
            if j < 0:
                return "".join(out), True
            i, in_comment = j + 2, False
            out.append(" ")
        elif line.startswith("/*", i):
            in_comment, i = True, i + 2
        elif line.startswith("//", i):
            break
        else:
            out.append(line[i])
            i += 1
    return "".join(out), in_comment


def strip_comments(text: str) -> str:
    """text with every comment replaced by a space, lines kept."""
    out, in_comment = [], False
    for line in text.splitlines(True):
        code, in_comment = split_code(line, in_comment)
        out.append(code if code.endswith("\n") else code + ("\n" if line.endswith("\n") else ""))
    return "".join(out)


@dataclass
class Chunk:
    """One declaration or preprocessor line, with the comment and blank
    lines before it (lead). code has the comments removed."""
    lead: list[str]
    lines: list[str]
    code: str


def depth(code: str) -> int:
    """Open braces and parentheses: a declaration ends at a ';' outside them."""
    return code.count("{") - code.count("}") + code.count("(") - code.count(")")


def chunks(text: str) -> tuple[list[Chunk], list[str]]:
    """Split text into declarations. Comment lines that continue the
    trailing comment of a declaration stay with it; other comment and blank
    lines lead the next declaration. Returns the chunks and the lines after
    the last one."""
    out: list[Chunk] = []
    lead: list[str] = []
    cur: list[str] = []
    code = ""
    in_comment = open_after = False
    lines = text.splitlines(True)
    i = 0
    while i < len(lines):
        line = lines[i]
        was_open = in_comment
        c, in_comment = split_code(line, in_comment)
        i += 1
        if not cur and not c.strip():
            if was_open and open_after and out and not lead:
                out[-1].lines.append(line)
                open_after = in_comment
            else:
                lead.append(line)
            continue
        if not cur and c.lstrip().startswith("#"):
            block = [line]
            guard = re.match(r"\s*#\s*ifndef\s+(\w+)", c)
            if guard and i < len(lines) and re.match(rf"\s*#\s*define\s+{guard.group(1)}\s*$", lines[i]):
                pass  # an include guard: its #endif closes the file, not this chunk
            elif re.match(r"\s*#\s*if", c):
                level = 1
                while level and i < len(lines):
                    block.append(lines[i])
                    level += bool(re.match(r"\s*#\s*if", lines[i])) - bool(re.match(r"\s*#\s*endif", lines[i]))
                    i += 1
            while block[-1].rstrip("\n").endswith("\\") and i < len(lines):
                block.append(lines[i])
                i += 1
            out.append(Chunk(lead, block, strip_comments("".join(block))))
            lead, open_after = [], False
            continue
        cur.append(line)
        code += c + " "
        if depth(code) == 0 and code.rstrip().endswith(";"):
            out.append(Chunk(lead, cur, " ".join(code.split())))
            lead, cur, code, open_after = [], [], "", in_comment
    if cur:
        raise MergeError("declaration without ';': " + "".join(cur).strip()[:60])
    return out, lead


def split_braces(lines: list[str]) -> tuple[list[str], list[str], list[str]]:
    """Head (up to the line with the first '{'), body, and tail (from the
    last line that starts with '}') of a struct or union declaration."""
    codes, in_comment = [], False
    for line in lines:
        c, in_comment = split_code(line, in_comment)
        codes.append(c)
    first = next(i for i, c in enumerate(codes) if "{" in c)
    last = max(i for i, c in enumerate(codes) if c.strip().startswith("}"))
    if last <= first:
        raise MergeError("struct on one line: " + lines[first].strip()[:60])
    return lines[:first + 1], lines[first + 1:last], lines[last:]


# ------------------------------------------------------------------ types

C_TYPES = {"char": 1, "short": 2, "int": 4, "long": 4, "long long": 8, "float": 4, "double": 8}


def align_up(n: int, a: int) -> int:
    return (n + a - 1) // a * a


class Types:
    """Sizes of the named types and values of the #defines in a set of
    headers, for laying out struct fields."""

    def __init__(self, texts: list[str]):
        self.structs: dict[str, tuple[str, str]] = {}   # name or tag -> (kind, body code)
        self.aliases: dict[str, tuple[str, str]] = {}   # typedef name -> type words, array lengths
        self.scalars: dict[str, int] = {}               # enums, function pointers
        self.defines: dict[str, str] = {}
        self.cache: dict[str, tuple[int, int]] = {}
        for text in texts:
            self.scan(strip_comments(text))

    def scan(self, code: str) -> None:
        for m in re.finditer(r"\b(typedef\s+)?(struct|union)\s*(\w*)\s*\{", code):
            end, level = m.end(), 1
            while level and end < len(code):
                level += {"{": 1, "}": -1}.get(code[end], 0)
                end += 1
            body = code[m.end():end - 1]
            if m.group(3):
                self.structs.setdefault("struct " + m.group(3), (m.group(2), body))
            name = re.match(r"\s*(\w*)\s*;", code[end:])
            if m.group(1) and name and name.group(1):
                self.structs.setdefault(name.group(1), (m.group(2), body))
        for m in re.finditer(r"typedef\s+enum\b[^{;]*\{[^}]*\}\s*(\w+)\s*;", code):
            self.scalars.setdefault(m.group(1), 4)
        for m in re.finditer(r"typedef[^;{]*\(\s*\*\s*(\w+)\s*\)\s*\(", code):
            self.scalars.setdefault(m.group(1), 4)
        for m in re.finditer(r"typedef\s+([A-Za-z_][\w \t]*?)\s*(\*?)\s*\b(\w+)\s*((?:\[[^\]]+\]\s*)*);", code):
            if m.group(2):
                self.scalars.setdefault(m.group(3), 4)
            else:
                self.aliases.setdefault(m.group(3), (m.group(1), m.group(4)))
        for m in re.finditer(r"^\s*#\s*define\s+(\w+)[ \t]+([^\n]+)$", code, re.M):
            self.defines.setdefault(m.group(1), m.group(2))
        for m in re.finditer(r"\benum\b[^{;]*\{([^}]*)\}", code):
            self.scan_enum(m.group(1))

    def scan_enum(self, body: str) -> None:
        """Enum constants, which array lengths use (CO_COUNT)."""
        value = -1
        for item in body.split(","):
            m = re.match(r"\s*(\w+)\s*(?:=\s*(.+?))?\s*$", item, re.S)
            if not m:
                continue
            try:
                value = self.value(m.group(2)) if m.group(2) else value + 1
            except MergeError:
                return  # an enum this merge cannot follow; its sizes stay unknown
            self.defines.setdefault(m.group(1), str(value))

    def size(self, words: list[str]) -> tuple[int, int]:
        """(size, alignment) of a type written as words: ['const', 'u8']."""
        words = [w for w in words if w not in ("const", "volatile", "signed")]
        if words[0] == "enum":
            return 4, 4
        if words[0] in ("struct", "union"):
            return self.named("struct " + words[1])
        plain = " ".join(w for w in words if w != "unsigned") or "int"
        if plain in C_TYPES:
            n = C_TYPES[plain]
            return n, n
        if len(words) != 1:
            raise MergeError("unknown type " + " ".join(words))
        return self.named(words[0])

    def named(self, name: str) -> tuple[int, int]:
        if name in self.cache:
            return self.cache[name]
        self.cache[name] = (0, 0)  # stops a loop on a self-referencing type
        if name in self.scalars:
            n = self.scalars[name]
            res = (n, n)
        elif name in self.aliases:
            words, arr = self.aliases[name]
            size, align = self.size(words.split())
            res = (size * dims(arr, self), align)
        elif name in self.structs or name.removeprefix("struct ") in self.structs:
            kind, body = self.structs.get(name) or self.structs[name.removeprefix("struct ")]
            res = layout(code_chunks(body), kind, self)[1:]
        else:
            raise MergeError("unknown type " + name)
        if res == (0, 0):
            raise MergeError("type with no size: " + name)
        self.cache[name] = res
        return res

    def value(self, expr: str, level: int = 0) -> int:
        """Value of an array length such as `0x1c - 0x02` or a #define."""
        if level > 10:
            raise MergeError("#define loop in " + expr)

        def name(m):
            if m.group(0) not in self.defines:
                raise MergeError("unknown constant " + m.group(0))
            return f"({self.value(self.defines[m.group(0)], level + 1)})"
        expr = re.sub(r"\b(0[xX][0-9a-fA-F]+|\d+)[uUlL]+\b", r"\1", expr)
        expr = re.sub(r"\b(?!0[xX])[A-Za-z_]\w*\b", name, expr)
        if not re.fullmatch(r"[\s0-9a-fA-FxX+\-*/%()<>|&~^]+", expr):
            raise MergeError("cannot evaluate " + expr)
        return int(eval(expr.replace("/", "//"), {"__builtins__": {}}))


# ----------------------------------------------------------------- layout

@dataclass
class Item:
    """A field, a bit-field group, a nested struct or union, or padding."""
    offset: int
    size: int
    align: int
    name: str | None          # None for padding
    lead: list[str]
    lines: list[str]
    code: str
    nested: str | None = None  # 'struct' or 'union' for a nested aggregate
    width: int | None = None   # bits used, for a bit-field group

    @property
    def text(self) -> str:
        return "".join(self.lines)


DECL = re.compile(r"^(.*?)\s*(\**)\s*(\w+)\s*((?:\[[^\]]+\]\s*)*)(?::\s*(\w+))?\s*;$")
FN_PTR = re.compile(r"\(\s*\*\s*(\w+)\s*((?:\[[^\]]+\]\s*)*)\)\s*\(")
NESTED = re.compile(r"^(struct|union)\s*\w*\s*\{.*\}\s*(\w*)\s*((?:\[[^\]]+\]\s*)*);$")


def declare(ch: Chunk, types: Types) -> Item:
    """Size, alignment and name of one declaration inside a struct."""
    m = NESTED.match(ch.code)
    if m:
        inner = ch.code[ch.code.index("{") + 1:ch.code.rindex("}")]
        _, size, align = layout(code_chunks(inner), m.group(1), types)
        name = m.group(2) or "@"
        return Item(0, size * dims(m.group(3), types), align, name, ch.lead, ch.lines, ch.code, m.group(1))
    m = FN_PTR.search(ch.code)
    if m:
        return Item(0, 4 * dims(m.group(2), types), 4, m.group(1), ch.lead, ch.lines, ch.code)
    m = DECL.match(ch.code)
    if m and not m.group(1) and m.group(5):
        # An unnamed bit-field such as `u16 : 12;`: it only takes up bits.
        size, align = types.size([m.group(3)])
        return Item(0, size, align, "(unnamed)", ch.lead, ch.lines, ch.code, width=types.value(m.group(5)))
    if not m or not m.group(1) or "," in ch.code:
        raise MergeError("cannot read field: " + ch.code)
    words, ptr, name, arr, bits = m.groups()
    size, align = (4, 4) if ptr else types.size(words.split())
    item = Item(0, size * dims(arr, types), align, name, ch.lead, ch.lines, ch.code)
    if bits is not None:
        item.width = types.value(bits)
    return item


def code_chunks(code: str) -> list[Chunk]:
    """Chunks of a body that has already lost its comments and may hold
    several declarations on one line."""
    return chunks(code.replace(";", ";\n"))[0]


def dims(text: str, types: Types) -> int:
    """Element count of array brackets such as `[4][2]`; 1 for none."""
    n = 1
    for d in re.findall(r"\[([^\]]+)\]", text or ""):
        n *= types.value(d)
    return n


def layout(chs: list[Chunk], kind: str, types: Types) -> tuple[list[Item], int, int]:
    """Place each declaration of a struct or union body. Returns the items,
    the size and the alignment. Consecutive bit-fields that share a storage
    unit become one item; comments in front of padding move to the next
    field, since padding is written again on a merge."""
    items: list[Item] = []
    off = end = 0
    align = 1
    carry: list[str] = []
    for ch in chs:
        it = declare(ch, types)
        it.lead = carry + it.lead
        carry = []
        last = items[-1] if items else None
        if (it.width is not None and last is not None and last.width is not None and kind == "struct"
                and last.size == it.size and it.width and last.width + it.width <= it.size * 8):
            last.lines += it.lead + it.lines
            last.code += " " + it.code
            last.name += "," + it.name
            last.width += it.width
            continue
        it.offset = 0 if kind == "union" else align_up(off, it.align)
        if it.name == "@":
            it.name = f"@{it.offset:x}"  # an anonymous struct or union: known by its offset
        if it.name.startswith("pad") and not it.nested:
            it.name = None
            carry, it.lead = it.lead, []
        items.append(it)
        off = it.offset + it.size
        end = max(end, off)
        align = max(align, it.align)
    return items, align_up(end, align), align


# ------------------------------------------------------------ struct merge

@dataclass
class Report:
    """What a merge did, for the summary line and for apply_commit."""
    added_fields: int = 0
    added_decls: int = 0
    renames: dict[str, str] = field(default_factory=dict)       # commit's field name -> mainline's
    type_renames: dict[str, str] = field(default_factory=dict)  # commit's type name -> mainline's
    notes: list[str] = field(default_factory=list)               # clashes, and what was kept
    conflicts: list[str] = field(default_factory=list)          # what a person has to fix

    def summary(self, path: str) -> str:
        parts = [f"+{self.added_fields} fields", f"+{self.added_decls} declarations"]
        parts += [f"renamed {a}->{b}" for a, b in self.renames.items()]
        parts += [f"type {a}->{b}" for a, b in self.type_renames.items()]
        return f"merged {path}: " + ", ".join(parts + self.notes)


def overlaps(a: Item, b: Item) -> bool:
    return a.offset < b.offset + b.size and b.offset < a.offset + a.size


def merge_body(kind: str, ours: list[str], theirs: list[str], base: list[str] | None,
               types: Types, rep: Report, where: str) -> list[str]:
    """Merge the lines between the braces of one struct or union."""
    o_chs, o_tail = chunks("".join(ours))
    o_items, o_size, o_align = layout(o_chs, kind, types)
    t_items, t_size, _ = layout(chunks("".join(theirs))[0], kind, types)
    b_items = layout(chunks("".join(base))[0], kind, types)[0] if base else []
    if kind == "union":
        return merge_union(o_items, t_items, b_items, o_tail, rep)
    named = [it for it in o_items if it.name]
    for t in t_items:
        if not t.name:
            continue
        if unchanged(t, b_items):
            # The commit did not touch it: mainline's version stays, and only
            # a rename on mainline matters to the commit's sources.
            o = next((o for o in named if o.offset == t.offset and o.size == t.size), None)
            if o is not None and o.name != t.name and simple(o.name) and simple(t.name):
                rep.renames[t.name] = o.name
            continue
        merge_field(named, t, b_items, types, rep, where)
    pads = {(p.offset, p.offset + p.size): p for p in t_items + o_items if p.name is None}
    end = max([o_size, t_size] + [it.offset + it.size for it in named])
    body = render(named, end, max(o_align, *(it.align for it in named), 1), pads, o_items) + o_tail
    check_layout(body, named, end, types)
    return body


def simple(name: str) -> bool:
    """A plain field name (not a bit-field group or an anonymous union)."""
    return "," not in name and not name.startswith(("@", "("))


def merge_field(named: list[Item], t: Item, base: list[Item], types: Types, rep: Report, where: str) -> None:
    """Add the commit's field t to mainline's fields `named`. Where both
    sides changed the same bytes, mainline's name stays so that its other
    users keep building; the commit's sources are rewritten to it
    (rep.renames)."""
    same = next((o for o in named if o.offset == t.offset and (o.size == t.size or o.name == t.name)), None)
    if same is not None:
        merge_same(named, same, t, base, types, rep, where)
        return
    clash = [o for o in named if overlaps(o, t)]
    if not clash:
        dup = [o for o in named if o.name == t.name]
        if dup:
            rep.conflicts.append(f"{where}.{t.name} is at 0x{dup[0].offset:x} on mainline"
                                 f" and at 0x{t.offset:x} in the commit")
            return
        named.append(t)
        rep.added_fields += 1
        return
    old = clash[0]
    if len(clash) == 1 and old.offset == t.offset and simple(old.name) and simple(t.name) \
            and unchanged(old, base):
        # The commit retyped a field mainline left as it was.
        named[named.index(old)] = renamed(t, old.name, rep)
        return
    names = ", ".join(f"{o.name} (0x{o.offset:x}, {o.size} bytes)" for o in clash)
    rep.conflicts.append(f"{where}.{t.name} (0x{t.offset:x}, {t.size} bytes) overlaps mainline's {names}")


def merge_same(named: list[Item], o: Item, t: Item, base: list[Item], types: Types, rep: Report, where: str) -> None:
    """Both sides declare the same bytes."""
    if o.text == t.text:
        return
    if o.nested and t.nested and o.name.startswith("@"):
        merge_nested(o, t, base, types, rep, where)
    elif unchanged(o, base):
        # Mainline left the field as it was in the base: the commit's edit
        # wins, as in a 3-way merge. Mainline's newer users of an old name
        # then fail to build; the summary names the field.
        named[named.index(o)] = t
        if o.name != t.name:
            rep.notes.append(f"took the commit's rename {where}.{o.name} -> {t.name}")
    elif o.name != t.name and simple(o.name) and simple(t.name):
        rep.renames[t.name] = o.name
        if o.code.replace(o.name, "") != t.code.replace(t.name, ""):
            rep.notes.append(f"kept mainline's {where}.{o.name} as `{o.code}` (the commit has `{t.code}`)")
    elif o.code != t.code:
        rep.notes.append(f"kept mainline's {where}.{o.name} as `{o.code}` (the commit has `{t.code}`)")


def unchanged(o: Item, base: list[Item]) -> bool:
    """Whether field o (of either side) is as the base had it."""
    return any(b.offset == o.offset and b.text == o.text for b in base)


def rename_keeping_columns(line: str, old: str, new: str) -> str:
    """Rename a field in its line, keeping a trailing comment in its column."""
    out = re.sub(rf"\b{re.escape(old)}\b", new, line, count=1)
    col = line.find("/*")
    m = re.search(r"\s+/\*", out)
    if col > 0 and m:
        out = out[:m.start()] + " " * max(1, col - m.start()) + out[m.end() - 2:]
    return out


def renamed(t: Item, name: str, rep: Report) -> Item:
    """The commit's field t under mainline's name."""
    if t.name != name:
        rep.renames[t.name] = name
        t.lines = [rename_keeping_columns(t.lines[0], t.name, name)] + t.lines[1:]
        t.name = name
    return t


def merge_nested(o: Item, t: Item, base: list[Item], types: Types, rep: Report, where: str) -> None:
    """Merge two anonymous unions or structs at the same offset."""
    oh, ob, ot = split_braces(o.lines)
    tb = split_braces(t.lines)[1]
    b = next((b for b in base if b.nested and b.name == o.name), None)
    bb = split_braces(b.lines)[1] if b is not None else None
    o.lines = oh + merge_body(o.nested, ob, tb, bb, types, rep, where) + ot
    o.code = " ".join(strip_comments(o.text).split())
    o.size = max(o.size, t.size)


def merge_union(ours: list[Item], theirs: list[Item], base: list[Item], tail: list[str], rep: Report) -> list[str]:
    """Members of a union all start at 0: merge them by name."""
    out = list(ours)
    by_name = {it.name: it for it in out}
    based = {it.name: it for it in base}
    for t in theirs:
        o = by_name.get(t.name)
        if o is None:
            out.append(t)
            rep.added_fields += 1
        elif o.text != t.text and t.name in based and based[t.name].text == o.text:
            out[out.index(o)] = t
    return [line for it in out for line in it.lead + it.lines] + tail


def pad_naming(items: list[Item]) -> tuple[int, int, str]:
    """How a struct names its offsets: the bias (a struct laid over another
    from 0x20 names its first byte pad_020), the hex width and the indent."""
    biases, width = Counter(), 2
    for it in items:
        m = re.search(r"\b(?:pad|unk)_([0-9a-f]+)\s*[\[;]", it.code)
        if m:
            biases[int(m.group(1), 16) - it.offset] += 1
            if len(m.group(1)) > 2 and m.group(1).startswith("0"):
                width = max(width, len(m.group(1)))  # zero-padded names: keep their width
    indent = next((re.match(r"\s*", it.lines[0]).group(0) for it in items), "    ")
    return (biases.most_common(1)[0][0] if biases else 0), width, indent


def pad_line(start: int, end: int, naming: tuple[int, int, str]) -> str:
    bias, width, indent = naming
    s, e = start + bias, end + bias
    if e - s == 1:
        return f"{indent}u8 pad_{s:0{width}x};\n"
    if s == 0:
        return f"{indent}u8 pad_{s:0{width}x}[0x{e:x}];\n"
    return f"{indent}u8 pad_{s:0{width}x}[0x{e:0{width}x} - 0x{s:0{width}x}];\n"


def render(named: list[Item], end: int, align: int, pads: dict, o_items: list[Item]) -> list[str]:
    """Lines of a struct body: the fields in offset order, with padding
    where the compiler would not add it by itself. A padding line of either
    side that fits a gap exactly is kept as it was."""
    naming = pad_naming(o_items) if o_items else (0, 2, "    ")
    out: list[str] = []
    cur = 0
    for it in sorted(named, key=lambda it: it.offset) + [None]:
        target = end if it is None else it.offset
        natural = align_up(cur, align if it is None else it.align)
        if target < cur:
            raise MergeError(f"fields overlap at 0x{target:x}")
        if target > cur:
            pad = pads.get((cur, target))
            if pad is not None:
                out += pad.lines
            elif target > natural:
                out.append(pad_line(cur, target, naming))
        if it is not None:
            out += it.lead + it.lines
            cur = it.offset + it.size
    return out


def check_layout(body: list[str], named: list[Item], end: int, types: Types) -> None:
    """Lay the merged body out again: every field must sit where it was meant to."""
    items, size, _ = layout(chunks("".join(body))[0], "struct", types)
    got = {it.name: it.offset for it in items if it.name}
    for it in named:
        if got.get(it.name) != it.offset:
            raise MergeError(f"{it.name} moved from 0x{it.offset:x} after the merge")
    if size != align_up(end, max([it.align for it in items] + [1])):
        raise MergeError(f"size changed to 0x{size:x}")


# ------------------------------------------------------------ header merge

@dataclass
class Entry:
    """A top-level declaration of a header, with the comments before it."""
    key: tuple
    lead: list[str]
    lines: list[str]
    code: str

    @property
    def text(self) -> str:
        return "".join(self.lines)

    @property
    def full(self) -> str:
        return "".join(self.lead + self.lines)


def entry_key(code: str) -> tuple:
    """What a top-level declaration declares, so both sides can be matched."""
    m = re.match(r"#\s*(define|undef)\s+(\w+)", code) or re.match(r"#\s*(include|ifndef|endif)\s*(\S*)", code)
    if m:
        return (m.group(1), m.group(2))
    if code.startswith("#"):
        return ("other", code)
    if "{" in code:
        m = re.search(r"\}\s*(\w+)\s*;$", code) if code.startswith("typedef") else None
        m = m or re.match(r"(?:typedef\s+)?(?:struct|union|enum)\s+(\w+)", code)
        if m:
            return ("type", m.group(1))
        return ("other", code)
    if code.startswith("typedef"):
        m = FN_PTR.search(code) or re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*;$", code)
        if re.match(r"typedef\s+(struct|union)\s+(\w+)\s+\2\s*;", code):
            return ("forward", m.group(1))
        return ("type", m.group(1))
    if code.startswith("extern") and not re.search(r"\w\s*\(", code.split("(*")[0]):
        m = FN_PTR.search(code) or re.search(r"(\w+)\s*(?:\[[^\]]*\]\s*)*;$", code)
        return ("var", m.group(1)) if m else ("other", code)
    m = re.search(r"(\w+)\s*\(", code)
    if m and code.endswith(");") and "(*" not in code:
        return ("func", m.group(1))
    return ("other", code)


def parse_header(text: str) -> tuple[list[Entry], list[str]]:
    """Top-level declarations of a header, keyed so that both sides match
    (see entry_key), and the lines after the last one."""
    chs, tail = chunks(text)
    entries = [Entry(entry_key(c.code.strip()), c.lead, c.lines, c.code.strip()) for c in chs]
    if len(entries) >= 2 and entries[0].key[0] == "ifndef" and entries[1].key == ("define", entries[0].key[1]):
        entries[0].key, entries[1].key = ("guard", 0), ("guard", 1)
    if entries and entries[-1].key[0] == "endif":
        entries[-1].key = ("guard", 2)
    seen = Counter()
    for e in entries:
        seen[e.key] += 1
        if seen[e.key] > 1:
            e.key = e.key + (seen[e.key],)
    return entries, tail


def extern_type(code: str) -> str | None:
    m = re.match(r"extern\s+(?:const\s+)?(?:struct\s+)?(\w+)", code)
    return m.group(1) if m else None


def find_aliases(ours: list[Entry], theirs: list[Entry]) -> dict[str, str]:
    """Structs the commit added under another name than mainline uses for
    the same data: both sides declare one extern, each with its own struct."""
    o = {e.key: e for e in ours}
    t_types = {e.key[1] for e in theirs if e.key[0] == "type"}
    out = {}
    for e in theirs:
        mine = o.get(e.key)
        if e.key[0] != "var" or mine is None or mine.code == e.code:
            continue
        a, b = extern_type(e.code), extern_type(mine.code)
        if a and b and a != b and a in t_types and ("type", a) not in o and ("type", b) in o:
            out[a] = b
    return out


def rename_words(text: str, names: dict[str, str]) -> str:
    for old, new in names.items():
        text = re.sub(rf"\b{re.escape(old)}\b", new, text)
    return text


def merge(ours: str, theirs: str, base: str, types: Types) -> tuple[str, Report]:
    """Merge the commit's version of a header (theirs) into mainline's (ours).
    Raises MergeError when a part cannot be read or laid out."""
    rep = Report()
    o_entries, o_tail = parse_header(ours)
    rep.type_renames = find_aliases(o_entries, parse_header(theirs)[0])
    t_entries = parse_header(rename_words(theirs, rep.type_renames))[0]
    b_by_key = {e.key: e for e in parse_header(rename_words(base, rep.type_renames))[0]}
    res = list(o_entries)
    t_keys = {e.key for e in t_entries}
    pos = start_position(res)
    for t in t_entries:
        keys = [e.key for e in res]
        if t.key in keys:
            i = keys.index(t.key)
            merge_entry(res[i], t, b_by_key.get(t.key), types, rep)
            pos = i + 1
        elif t.key in b_by_key:
            if b_by_key[t.key].text != t.text:
                rep.notes.append(f"kept mainline's removal of {t.key[1]}")
        else:
            i = min(pos, guard_end(res))
            res.insert(i, t)
            pos = i + 1
            rep.added_decls += 1
    for key, b in b_by_key.items():
        gone = key not in t_keys and key[0] != "guard"
        mine = next((e for e in res if e.key == key), None)
        if gone and mine is not None and mine.text == b.text:
            res.remove(mine)
    text = "".join(line for e in res for line in e.lead + e.lines) + "".join(o_tail)
    return text, rep


def start_position(entries: list[Entry]) -> int:
    """Where declarations go that have no neighbour on mainline: after the
    include guard and the #include lines."""
    i = 0
    while i < len(entries) and entries[i].key[0] in ("guard", "include"):
        i += 1
    return i


def guard_end(entries: list[Entry]) -> int:
    return len(entries) - 1 if entries and entries[-1].key == ("guard", 2) else len(entries)


def merge_entry(o: Entry, t: Entry, b: Entry | None, types: Types, rep: Report) -> None:
    """Merge the commit's version t of a declaration into mainline's o."""
    if o.full == t.full:
        return
    if b is not None and b.full == o.full:
        o.lead, o.lines = t.lead, t.lines  # only the commit changed it
        return
    if b is not None and b.lead == o.lead:
        o.lead = t.lead
    if o.key[0] == "type" and "{" in o.code and "{" in t.code and not o.code.startswith(("enum", "typedef enum")):
        kind = "union" if re.match(r"(typedef\s+)?union\b", o.code) else "struct"
        oh, ob, ot = split_braces(o.lines)
        tb = split_braces(t.lines)[1]
        bb = split_braces(b.lines)[1] if b is not None and "{" in b.code else None
        o.lines = oh + merge_body(kind, ob, tb, bb, types, rep, o.key[1]) + ot
    elif (b is None or b.text != t.text) and o.code != t.code:
        rep.notes.append(f"kept mainline's {o.key[1] if len(o.key) > 1 else o.code[:40]}")


def declared_functions(text: str) -> set[str]:
    """Names of the functions a header declares."""
    try:
        return {e.key[1] for e in parse_header(text)[0] if e.key[0] == "func"}
    except MergeError:
        return set(re.findall(r"^\s*[\w\s\*]+?\b(\w+)\s*\([^;{]*\)\s*;", text, re.M))
