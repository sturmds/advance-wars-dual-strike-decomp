"""Merge two versions of a C header by meaning. apply_commit.py uses it when
`git merge-file` cannot merge a header under include/.

Agents add fields to the same shared structs at the same time. A text merge
conflicts as soon as two of them split the same padding array, so this
reads both versions as declarations instead of lines:

- Struct fields are placed by byte offset. The result has the named fields
  of both sides, with padding written again between them. Where both sides
  name the same bytes differently, mainline's name stays and the pair is
  returned so that apply_commit.py can rewrite the commit's sources.
- Offsets come from mwccarm when merge() gets a probe (apply_commit.py
  always passes one): each side's header is compiled in that side's tree,
  field types take the sizes and alignments the compiler reports, and each
  side's layout and the merged one must match the compiler's field by
  field, or the merge raises MergeError and the conflict is left to a
  person (b31: an OSAlarm placed 8 bytes late, and an OSThread sized with
  ARM9's definition in an ARM7 header).
- Typedefs, structs, #defines, externs and prototypes that only one side
  has are kept. Where both sides changed the same one, mainline's version
  stays and the clash is noted; for a #define with two different values
  it is a conflict.
- Enums are merged constant by constant: the result has the constants of
  both sides, the commit's new ones written with their values. A constant
  with two different values is a conflict.
- A struct the commit names differently from mainline (both sides declare
  the same extern with it) is merged into mainline's struct.
- Fields that overlap without matching are reported as conflicts.
- A field the base has and one side removed stays removed, and so does the
  base's size when one side shrank the struct (b23/mb6: the commit's older
  copy put back DlPlayHostMsgHead.pad_06). If the other side changed that
  field, it is a conflict.

The base (the commit's parent version, empty for a header both sides
added) tells an edit from an addition: where mainline still has the base's
version of a field or declaration, the commit's version wins, as in a
3-way merge.
"""

import re
import subprocess
from collections import Counter
from collections.abc import Callable
from dataclasses import dataclass, field
from pathlib import Path


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

# (size, alignment). mwccarm aligns the 8-byte types to 4, with the ARM9 and
# the ARM7 flags alike (b31/wlq2: an OSAlarm, which holds a u64, sits at
# WlWork+0x608).
C_TYPES = {"char": (1, 1), "short": (2, 2), "int": (4, 4), "long": (4, 4), "long long": (8, 4),
           "float": (4, 4), "double": (8, 4)}


def align_up(n: int, a: int) -> int:
    return (n + a - 1) // a * a


class Types:
    """Sizes of the named types and values of the #defines in a set of
    headers, for laying out struct fields. sizes, when given, holds what
    the compiler reports for the field types of the header being merged
    (Compiled.sizes); it wins over sizes worked out from the texts, which
    do not know which of two definitions of a type the header includes
    (b31/wlc0: ARM9's OSThread of 148 bytes in place of ARM7's 0x78)."""

    def __init__(self, texts: list[str], sizes: dict[str, tuple[int, int]] | None = None):
        self.sizes = sizes or {}
        self.structs: dict[str, tuple[str, str]] = {}   # name or tag -> (kind, body code)
        self.aliases: dict[str, tuple[str, str]] = {}   # typedef name -> type words, array lengths
        self.scalars: dict[str, int] = {}               # enums, function pointers
        self.defines: dict[str, str] = {}
        self.cache: dict[str, tuple[int, int]] = {}
        self.texts = list(texts)
        for text in texts:
            self.scan(strip_comments(text))

    def first(self, *texts: str) -> "Types":
        """These types with the definitions in texts taking precedence."""
        return Types(list(texts) + self.texts, self.sizes)

    def with_sizes(self, sizes: dict[str, tuple[int, int]]) -> "Types":
        """These types with the compiler's sizes."""
        return Types(self.texts, sizes)

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
        written = " ".join(w for w in words if w not in ("const", "volatile"))
        if written in self.sizes:
            return self.sizes[written]
        words = [w for w in words if w not in ("const", "volatile", "signed")]
        if words[0] == "enum":
            return 4, 4
        if words[0] in ("struct", "union"):
            return self.named("struct " + words[1])
        plain = " ".join(w for w in words if w != "unsigned") or "int"
        if plain in C_TYPES:
            return C_TYPES[plain]
        if len(words) != 1:
            raise MergeError("unknown type " + " ".join(words))
        return self.named(words[0])

    def named(self, name: str) -> tuple[int, int]:
        if name in self.cache:
            return self.cache[name]
        self.cache[name] = (0, 0)  # stops a loop on a self-referencing type
        if name in self.sizes:
            res = self.sizes[name]
        elif name in self.scalars:
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
        if it.name.startswith("pad") and not it.nested and it.width is None:
            # Padding is written again on a merge. A bit-field named pad_*
            # stays a field: the bit-fields after it share its storage unit.
            it.name = None
            carry, it.lead = it.lead, []
        items.append(it)
        off = it.offset + it.size
        end = max(end, off)
        align = max(align, it.align)
    return items, align_up(end, align), align


# ------------------------------------------------------------ struct merge

@dataclass
class Sides:
    """Types to lay out each version with. A struct one side embeds can
    have another size on the other side (b12/o4s2: mainline had grown
    Unk_ov004_022bf158_818 by 4 bytes, and the commit's fields after it
    were placed 4 bytes late)."""
    ours: Types
    theirs: Types
    base: Types
    compiled: dict[str, "Compiled"] = field(default_factory=dict)  # 'ours', 'theirs', 'base'


@dataclass
class Report:
    """What a merge did, for the summary line and for apply_commit."""
    added_fields: int = 0
    added_decls: int = 0
    renames: dict[str, str] = field(default_factory=dict)       # commit's field name -> mainline's
    rename_structs: dict[str, str] = field(default_factory=dict)  # commit's field name -> its struct
    type_renames: dict[str, str] = field(default_factory=dict)  # commit's type name -> mainline's
    notes: list[str] = field(default_factory=list)               # clashes, and what was kept
    conflicts: list[str] = field(default_factory=list)          # what a person has to fix
    merged_structs: list[tuple[str, str]] = field(default_factory=list)  # (name, kind) laid out again

    def rename(self, old: str, new: str, where: str) -> None:
        self.renames[old] = new
        self.rename_structs[old] = where

    def summary(self, path: str) -> str:
        parts = [f"+{self.added_fields} fields", f"+{self.added_decls} declarations"]
        parts += [f"renamed {a}->{b}" for a, b in self.renames.items()]
        parts += [f"type {a}->{b}" for a, b in self.type_renames.items()]
        return f"merged {path}: " + ", ".join(parts + self.notes)


def overlaps(a: Item, b: Item) -> bool:
    return a.offset < b.offset + b.size and b.offset < a.offset + a.size


def merge_body(kind: str, ours: list[str], theirs: list[str], base: list[str] | None,
               sides: Sides, rep: Report, where: str) -> list[str]:
    """Merge the lines between the braces of one struct or union. Each
    side is laid out with its own types: a struct it embeds may have
    another size on the other side."""
    o_chs, o_tail = chunks("".join(ours))
    o_items, o_size, o_align = layout(o_chs, kind, sides.ours)
    t_items, t_size, _ = layout(chunks("".join(theirs))[0], kind, sides.theirs)
    b_items, b_size, _ = layout(chunks("".join(base))[0], kind, sides.base) if base else ([], None, 1)
    if kind == "union":
        return merge_union(o_items, t_items, b_items, o_tail, sides, rep, where)
    named = [it for it in o_items if it.name and not removed(it, t_items, b_items)]
    for t in t_items:
        if not t.name:
            continue
        if unchanged(t, b_items):
            # The commit did not touch it: mainline's version stays, and only
            # a rename on mainline matters to the commit's sources.
            o = next((o for o in named if o.offset == t.offset and o.size == t.size), None)
            if o is not None and o.name != t.name and simple(o.name) and simple(t.name):
                rep.rename(t.name, o.name, where)
            continue
        merge_field(named, t, b_items, sides, rep, where)
    pads = {(p.offset, p.offset + p.size): p for p in t_items + o_items if p.name is None}
    # The size changes as in a 3-way merge: the side that kept the base's
    # size follows the other one. Fields still decide the least size.
    sizes = [t_size] if o_size == b_size else [o_size] if t_size == b_size else [o_size, t_size]
    end = max(sizes + [it.offset + it.size for it in named])
    body = render(named, end, max(o_align, *(it.align for it in named), 1), pads, o_items) + o_tail
    check_layout(body, named, end, sides.ours)
    return body


def removed(o: Item, theirs: list[Item], base: list[Item]) -> bool:
    """Whether the commit removed mainline's field o: o is as the base had
    it, and no named field of the commit covers its bytes."""
    return unchanged(o, base) and not any(t.name and overlaps(o, t) for t in theirs)


def simple(name: str) -> bool:
    """A plain field name (not a bit-field group or an anonymous union)."""
    return "," not in name and not name.startswith(("@", "("))


def merge_field(named: list[Item], t: Item, base: list[Item], sides: Sides, rep: Report, where: str) -> None:
    """Add the commit's field t to mainline's fields `named`. Where both
    sides changed the same bytes, mainline's name stays so that its other
    users keep building; the commit's sources are rewritten to it
    (rep.renames)."""
    same = next((o for o in named if o.offset == t.offset and (o.size == t.size or o.name == t.name)), None)
    if same is not None:
        merge_same(named, same, t, base, sides, rep, where)
        return
    clash = [o for o in named if overlaps(o, t)]
    gone = [b for b in base if b.name and overlaps(b, t)] if not clash else []
    if gone:
        # Mainline removed the base's field here, and the commit changed it.
        names = ", ".join(f"{b.name} (0x{b.offset:x}, {b.size} bytes)" for b in gone)
        rep.conflicts.append(f"{where}.{t.name} (0x{t.offset:x}, {t.size} bytes): mainline removed"
                             f" the base's {names}, and the commit changed it")
        return
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
        named[named.index(old)] = renamed(t, old.name, rep, where)
        return
    names = ", ".join(f"{o.name} (0x{o.offset:x}, {o.size} bytes)" for o in clash)
    rep.conflicts.append(f"{where}.{t.name} (0x{t.offset:x}, {t.size} bytes) overlaps mainline's {names}")


def merge_same(named: list[Item], o: Item, t: Item, base: list[Item], sides: Sides, rep: Report,
               where: str) -> None:
    """Both sides declare the same bytes."""
    if o.text == t.text:
        return
    if o.nested and t.nested and o.name.startswith("@"):
        merge_nested(o, t, base, sides, rep, where)
    elif unchanged(o, base):
        # Mainline left the field as it was in the base: the commit's edit
        # wins, as in a 3-way merge. Mainline's newer users of an old name
        # then fail to build; the summary names the field.
        named[named.index(o)] = t
        if o.name != t.name and simple(o.name) and simple(t.name):
            rep.notes.append(f"took the commit's rename {where}.{o.name} -> {t.name}")
    elif o.name != t.name and simple(o.name) and simple(t.name):
        rep.rename(t.name, o.name, where)
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


def renamed(t: Item, name: str, rep: Report, where: str) -> Item:
    """The commit's field t under mainline's name."""
    if t.name != name:
        rep.rename(t.name, name, where)
        t.lines = [rename_keeping_columns(t.lines[0], t.name, name)] + t.lines[1:]
        t.name = name
    return t


def merge_nested(o: Item, t: Item, base: list[Item], sides: Sides, rep: Report, where: str) -> None:
    """Merge two anonymous unions or structs at the same offset."""
    oh, ob, ot = split_braces(o.lines)
    tb = split_braces(t.lines)[1]
    b = next((b for b in base if b.nested and b.name == o.name), None)
    bb = split_braces(b.lines)[1] if b is not None else None
    o.lines = oh + merge_body(o.nested, ob, tb, bb, sides, rep, where) + ot
    o.code = " ".join(strip_comments(o.text).split())
    o.size = max(o.size, t.size)


def merge_union(ours: list[Item], theirs: list[Item], base: list[Item], tail: list[str],
                sides: Sides, rep: Report, where: str) -> list[str]:
    """Members of a union all start at 0: merge them by name. Two versions
    of an anonymous struct member are merged field by field."""
    t_names = {it.name for it in theirs}
    based = {it.name: it for it in base}
    # A member the commit removed and mainline left as it was goes.
    out = [it for it in ours if not (it.name not in t_names and it.name in based
                                     and based[it.name].text == it.text)]
    by_name = {it.name: it for it in out}
    for t in theirs:
        o = by_name.get(t.name)
        if o is None and t.name in based:
            if based[t.name].text != t.text:
                rep.conflicts.append(f"{where}.{t.name}: mainline removed it from the union,"
                                     f" and the commit changed it")
            continue  # mainline removed it
        if o is None:
            out.append(t)
            rep.added_fields += 1
        elif o.text == t.text:
            continue
        elif t.name in based and based[t.name].text == o.text:
            out[out.index(o)] = t
        elif o.nested and t.nested and o.name.startswith("@"):
            merge_nested(o, t, base, sides, rep, where)
        elif o.code != t.code:
            rep.notes.append(f"kept mainline's {where}.{o.name} as `{o.code}` (the commit has `{t.code}`)")
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


# --------------------------------------------------------- compiler probe

@dataclass
class Compiled:
    """What mwccarm makes of one version of a header: (size, alignment) of
    each type its struct fields are declared with, and each struct's size
    and field offsets (members of anonymous structs and unions listed
    beside the others)."""
    sizes: dict[str, tuple[int, int]]
    structs: dict[str, tuple[int, dict[str, int]]]


def is_aggregate(e: "Entry") -> bool:
    return e.key[0] == "type" and "{" in e.code and not re.match(r"(typedef\s+)?enum\b", e.code)


def struct_entries(text: str) -> dict[str, str]:
    """Structs and unions a header defines: entry name -> how C names the
    type (`Foo`, or `struct Foo` for a struct without a typedef)."""
    out = {}
    for e in parse_header(text)[0]:
        kind = re.match(r"(?:typedef\s+)?(struct|union)\b", e.code)
        if is_aggregate(e) and kind:
            typedef = e.code.startswith("typedef") and re.search(r"\}\s*\w+\s*;$", e.code)
            out[e.key[1]] = e.key[1] if typedef else f"{kind.group(1)} {e.key[1]}"
    return out


def field_types(text: str) -> set[str]:
    """The types the struct fields of a header are declared with, as
    written (`u16`, `OSAlarm`, `struct Foo`), pointers left out."""
    out: set[str] = set()

    def walk(body: str) -> None:
        for ch in code_chunks(body):
            if NESTED.match(ch.code):
                walk(ch.code[ch.code.index("{") + 1:ch.code.rindex("}")])
            elif not FN_PTR.search(ch.code):
                m = DECL.match(ch.code)
                if m and not m.group(2):
                    words = (m.group(1) or m.group(3)).split()
                    written = " ".join(w for w in words if w not in ("const", "volatile"))
                    if written not in ("", "struct", "union", "enum"):
                        out.add(written)
    for e in parse_header(text)[0]:
        if is_aggregate(e):
            walk(e.code[e.code.index("{") + 1:e.code.rindex("}")])
    return out


def probe_source(rel: str, text: str) -> tuple[str, list[str], list[str]]:
    """A file that includes the header at rel and declares one pointer per
    struct of text and one per field type T, the latter inside
    `struct { char c; T t; }` (t's offset is T's alignment). Returns it
    with the probed struct names and field types, in the pointers' order."""
    structs = struct_entries(text)
    types = sorted(field_types(text))
    lines = ['#include "types.h"', f'#include "{rel}"']
    lines += [f"{c} *hm_s{i};" for i, c in enumerate(structs.values())]
    lines += [f"struct hm_a{i} {{ char c; {t} t; }} *hm_t{i};" for i, t in enumerate(types)]
    return "\n".join(lines) + "\n", list(structs), types


def read_probe(dwarf: str, structs: list[str], types: list[str]) -> Compiled:
    """Compiled from the DWARF of a compiled probe_source."""
    import dsindex  # imported here: it reads configure.py, which the tests' repos lack
    dies = dsindex.parse_dies(dwarf)
    dt = dsindex.Types(dies)
    target = {}
    for d in dies.values():
        if d["tag"] == "variable" and d["attrs"].get("name", "").startswith("hm_"):
            ptr = dt.ref(d)
            target[d["attrs"]["name"]] = dt.strip(dt.ref(dies[ptr])) if ptr is not None else None
    out = Compiled({}, {})
    for i, name in enumerate(structs):
        off = target.get(f"hm_s{i}")
        if off is None:
            raise MergeError(f"mwccarm did not report the layout of {name}")
        rec = dt.aggregate(off)
        out.structs[name] = (rec["size"], {f["name"]: f["offset"] for f in rec["fields"] if f["name"]})
    for i, t in enumerate(types):
        off = target.get(f"hm_t{i}")
        member = next((dt.member(c) for c in dies[off]["children"] if dies[c]["attrs"].get("name") == "t"),
                      None) if off is not None else None
        if member is None:
            raise MergeError(f"mwccarm did not report the size of {t}")
        out.sizes[t] = (member["size"], member["offset"])
    return out


def compile_header(cc: Path, flags: str, include: Path, rel: str, text: str) -> Compiled:
    """Write text to include/rel and compile a probe of it with mwccarm:
    flags with include in place of their `-i` directory, and -g. Raises
    MergeError with the compiler's message when it does not compile."""
    (include / rel).parent.mkdir(parents=True, exist_ok=True)
    (include / rel).write_text(text)
    src, structs, types = probe_source(rel, text)
    probe, obj = include.parent / "hm_probe.c", include.parent / "hm_probe.o"
    probe.write_text(src)
    args = flags.split()
    if "-i" in args:
        del args[args.index("-i"):args.index("-i") + 2]
    r = subprocess.run([str(cc), "-c", *args, "-i", str(include), "-g", "-o", str(obj), str(probe)],
                       cwd=include.parent, capture_output=True, text=True)
    if r.returncode or not obj.exists():
        msg = " ".join((r.stdout + r.stderr).split())[:300]
        raise MergeError(f"mwccarm cannot compile {rel} to check the layout: {msg}")
    d = subprocess.run(["arm-none-eabi-objdump", "--dwarf=info", str(obj)],
                       capture_output=True, text=True, check=True)
    obj.unlink()
    return read_probe(d.stdout, structs, types)


def flat_offsets(items: list[Item], types: Types, base: int = 0) -> dict[str, tuple[int, int]]:
    """Field name -> (offset, size) of laid-out items, the members of
    anonymous structs and unions included at their own offsets. Each name
    of a bit-field group gets the group's storage unit."""
    out: dict[str, tuple[int, int]] = {}
    for it in items:
        if not it.name or it.name == "(unnamed)":
            continue
        if it.name.startswith("@"):
            inner = it.code[it.code.index("{") + 1:it.code.rindex("}")]
            out.update(flat_offsets(layout(code_chunks(inner), it.nested, types)[0], types, base + it.offset))
            continue
        for name in it.name.split(","):
            if name != "(unnamed)":
                out[name] = (base + it.offset, it.size)
    return out


def compare_layout(where: str, side: str, items: list[Item], size: int, types: Types,
                   compiled: Compiled) -> None:
    """Raise MergeError where the compiler places a field of struct where
    elsewhere than this merge does, or gives the struct another size. A
    bit-field only has to fall inside its storage unit."""
    if where not in compiled.structs:
        raise MergeError(f"mwccarm did not report the layout of {where} ({side})")
    c_size, c_fields = compiled.structs[where]
    for name, (off, n) in flat_offsets(items, types).items():
        got = c_fields.get(name)
        bits = "," in name or any(it.width is not None and name in it.name.split(",") for it in items)
        if got != off and not (bits and got is not None and off <= got < off + n):
            raise MergeError(f"{side}: header_merge places {where}.{name} at 0x{off:x}, mwccarm at "
                             + (f"0x{got:x}" if got is not None else "no offset")
                             + "; refusing to write a layout that differs from the compiler's")
    if c_size != size:
        raise MergeError(f"{side}: header_merge makes {where} 0x{size:x} bytes, mwccarm 0x{c_size:x};"
                         " refusing to write a layout that differs from the compiler's")


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
        m = re.match(r"enum\s*\{\s*(\w+)", code)
        if m:
            return ("enum", m.group(1))  # an anonymous enum: known by its first constant
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
    """Rename every word that is a key of names, all in one pass: with
    {A: B, C: A} an A becomes B and a C becomes A, never B."""
    if not names:
        return text
    words = re.compile(r"\b(?:" + "|".join(re.escape(k) for k in sorted(names, key=len, reverse=True)) + r")\b")
    return words.sub(lambda m: names[m.group(0)], text)


Probe = Callable[[str, str], Compiled]


def merge(ours: str, theirs: str, base: str, types: Types, theirs_types: Types | None = None,
          probe: Probe | None = None) -> tuple[str, Report]:
    """Merge the commit's version of a header (theirs) into mainline's (ours).
    types has mainline's type definitions first, theirs_types the commit's
    (by default types with theirs read first). Raises MergeError when a
    part cannot be read or laid out.

    probe(side, text) compiles one version of the header in that side's
    tree ('ours', 'theirs', 'base' or 'merged'). With it, field types take
    their sizes from the compiler, each side's layout and the merged one
    must match the compiler's, and a mismatch raises MergeError."""
    rep = Report()
    o_entries, o_tail = parse_header(ours)
    rep.type_renames = find_aliases(o_entries, parse_header(theirs)[0])
    sides = Sides(types, theirs_types or types.first(rename_words(theirs, rep.type_renames)), types.first(base))
    t_entries = parse_header(rename_words(theirs, rep.type_renames))[0]
    b_by_key = {e.key: e for e in parse_header(rename_words(base, rep.type_renames))[0]}
    if probe is not None and merges_fields(o_entries, t_entries, b_by_key):
        sides.compiled = {"ours": probe("ours", ours), "theirs": renamed_compiled(probe("theirs", theirs),
                                                                                 rep.type_renames)}
        if base.strip():
            sides.compiled["base"] = renamed_compiled(probe("base", base), rep.type_renames)
        # Mainline's types also lay out the merged struct, which can have
        # fields of types only the commit uses (b31/wlc0: WlWork.thread).
        sides.ours = sides.ours.with_sizes({**sides.compiled["theirs"].sizes, **sides.compiled["ours"].sizes})
        sides.theirs = sides.theirs.with_sizes(sides.compiled["theirs"].sizes)
        sides.base = sides.base.with_sizes(sides.compiled.get("base", sides.compiled["ours"]).sizes)
    res = list(o_entries)
    t_keys = {e.key for e in t_entries}
    pos = start_position(res)
    for t in t_entries:
        keys = [e.key for e in res]
        if t.key in keys:
            i = keys.index(t.key)
            merge_entry(res[i], t, b_by_key.get(t.key), sides, rep)
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
    if probe is not None and rep.merged_structs:
        check_merged(text, probe("merged", text), sides, rep)
    return text, rep


def merges_fields(ours: list[Entry], theirs: list[Entry], base: dict[tuple, Entry]) -> bool:
    """Whether merge_entry will merge some struct or union field by field:
    both sides changed it."""
    o_by = {e.key: e for e in ours}
    for t in theirs:
        o, b = o_by.get(t.key), base.get(t.key)
        if o is not None and is_aggregate(o) and "{" in t.code and o.full != t.full \
                and not (b is not None and b.full == o.full):
            return True
    return False


def renamed_compiled(c: Compiled, names: dict[str, str]) -> Compiled:
    """c with the commit's type names replaced by mainline's (find_aliases)."""
    if not names:
        return c
    return Compiled({rename_words(t, names): v for t, v in c.sizes.items()},
                    {names.get(s, s): v for s, v in c.structs.items()})


def check_merged(text: str, compiled: Compiled, sides: Sides, rep: Report) -> None:
    """Every struct merged field by field must come out of the compiler as
    this merge laid it out."""
    entries = {e.key: e for e in parse_header(text)[0]}
    types = sides.ours.with_sizes(compiled.sizes)
    for name, kind in rep.merged_structs:
        e = entries[("type", name)]
        items, size, _ = layout(chunks("".join(split_braces(e.lines)[1]))[0], kind, types)
        compare_layout(name, "merged", items, size, types, compiled)


def start_position(entries: list[Entry]) -> int:
    """Where declarations go that have no neighbour on mainline: after the
    include guard and the #include lines."""
    i = 0
    while i < len(entries) and entries[i].key[0] in ("guard", "include"):
        i += 1
    return i


def guard_end(entries: list[Entry]) -> int:
    return len(entries) - 1 if entries and entries[-1].key == ("guard", 2) else len(entries)


def merge_entry(o: Entry, t: Entry, b: Entry | None, sides: Sides, rep: Report) -> None:
    """Merge the commit's version t of a declaration into mainline's o."""
    if o.full == t.full:
        return
    if b is not None and b.full == o.full:
        o.lead, o.lines = t.lead, t.lines  # only the commit changed it
        return
    if b is not None and b.lead == o.lead:
        o.lead = t.lead
    if "{" in o.code and "{" in t.code and o.code.startswith(("enum", "typedef enum")):
        o.lines = merge_enum(o, t, b, sides, rep)
    elif o.key[0] == "type" and "{" in o.code and "{" in t.code:
        kind = "union" if re.match(r"(typedef\s+)?union\b", o.code) else "struct"
        oh, ob, ot = split_braces(o.lines)
        tb = split_braces(t.lines)[1]
        bb = split_braces(b.lines)[1] if b is not None and "{" in b.code else None
        check_sides(kind, o.key[1], {"ours": ob, "theirs": tb, "base": bb}, sides)
        o.lines = oh + merge_body(kind, ob, tb, bb, sides, rep, o.key[1]) + ot
        rep.merged_structs.append((o.key[1], kind))
    elif o.key[0] == "define" and (b is None or b.text != t.text) and define_body(o) != define_body(t):
        rep.conflicts.append(f"#define {o.key[1]} is `{define_body(o)}` on mainline"
                             f" and `{define_body(t)}` in the commit")
    elif (b is None or b.text != t.text) and o.code != t.code:
        rep.notes.append(f"kept mainline's {o.key[1] if len(o.key) > 1 else o.code[:40]}")


def check_sides(kind: str, where: str, bodies: dict[str, list[str] | None], sides: Sides) -> None:
    """Before a struct is merged field by field: each side's version must be
    laid out here as the compiler lays it out in that side's tree, since
    the merge places fields by these offsets."""
    for side, body in bodies.items():
        compiled = sides.compiled.get(side)
        if body is None or compiled is None:
            continue
        types = getattr(sides, side)
        items, size, _ = layout(chunks("".join(body))[0], kind, types)
        compare_layout(where, side, items, size, types, compiled)


def define_body(e: Entry) -> str:
    """What a #define expands to, with comments and extra spaces removed."""
    code = re.sub(r"^\s*#\s*define\s+\w+", "", e.code.replace("\\\n", " "))
    return " ".join(code.split())


# ------------------------------------------------------------- enum merge

@dataclass
class Member:
    """One enum constant: its value, whether it is written `= expr`, and
    the index of its line in the enum body."""
    name: str
    value: int
    explicit: bool
    line: int


def enum_members(body: list[str], types: Types) -> list[Member]:
    out: list[Member] = []
    local: dict[str, int] = {}
    value, in_comment = -1, False
    for i, line in enumerate(body):
        code, in_comment = split_code(line, in_comment)
        for item in code.split(","):
            if not item.strip():
                continue
            m = re.fullmatch(r"\s*(\w+)\s*(?:=\s*(.+?))?\s*", item, re.S)
            if not m:
                raise MergeError("cannot read enum constant: " + item.strip())
            if m.group(2):
                expr = re.sub(r"\b[A-Za-z_]\w*\b", lambda n: str(local.get(n.group(0), n.group(0))), m.group(2))
                value = types.value(expr)
            else:
                value += 1
            local[m.group(1)] = value
            out.append(Member(m.group(1), value, bool(m.group(2)), i))
    return out


def merge_enum(o: Entry, t: Entry, b: Entry | None, sides: Sides, rep: Report) -> list[str]:
    """Merge two versions of an enum as the union of their constants. A
    constant only the commit has is written with its value (`NAME = 24,`)
    and placed by value, so no constant of mainline changes value. A
    constant the two sides give different values is a conflict, unless
    mainline still has the base's value (then the commit's value wins)."""
    where = o.key[1]
    oh, ob, ot = split_braces(o.lines)
    tb = split_braces(t.lines)[1]
    bb = split_braces(b.lines)[1] if b is not None and "{" in b.code else []
    ours, theirs, base = (enum_members(x, ts) for x, ts in ((ob, sides.ours), (tb, sides.theirs), (bb, sides.base)))
    o_by, b_by = {m.name: m for m in ours}, {m.name: m for m in base}
    body = list(ob)
    inserts: dict[int, list[tuple[int, str]]] = {}  # index in ob to insert before -> (value, line)
    for m in theirs:
        mine = o_by.get(m.name)
        if mine is not None:
            old = b_by.get(m.name)
            if mine.value == m.value or (old is not None and old.value == m.value):
                continue
            if old is not None and old.value == mine.value and alone(ours, mine) and alone(theirs, m):
                body[mine.line] = set_comma(tb[m.line], has_comma(ob[mine.line]))
            else:
                rep.conflicts.append(f"enum {where}: {m.name} is {mine.value} on mainline"
                                     f" and {m.value} in the commit")
            continue
        if m.name in b_by:
            rep.notes.append(f"kept mainline's removal of {m.name}")
            continue
        same = [x.name for x in ours if x.value == m.value]
        if same:
            rep.notes.append(f"{m.name} and {', '.join(same)} are both {m.value} in {where}")
        before = next((x.line for x in ours if x.explicit and x.value > m.value), len(ob))
        indent = re.match(r"[ \t]*", tb[m.line]).group(0) or "    "
        comment = trailing_comment(tb[m.line]) if alone(theirs, m) else ""
        inserts.setdefault(before, []).append((m.value, f"{indent}{m.name} = {m.value},"
                                                        f"{' ' + comment if comment else ''}\n"))
        rep.added_decls += 1
    last = max((x.line for x in ours), default=-1)
    for before in sorted(inserts, reverse=True):
        lines = [line for _, line in sorted(inserts[before])]
        if before > last >= 0:
            # After the last constant: that one gains a comma, the last new one has none.
            body[last] = set_comma(body[last], True)
            lines[-1] = set_comma(lines[-1], False)
            body[last + 1:last + 1] = lines
        else:
            body[before:before] = lines
    return oh + body + ot


def alone(members: list[Member], m: Member) -> bool:
    """Whether m is the only constant on its line."""
    return sum(x.line == m.line for x in members) == 1


def trailing_comment(line: str) -> str:
    i = line.find("/*")
    return line[i:].rstrip("\n") if i >= 0 else ""


def has_comma(line: str) -> bool:
    return split_code(line, False)[0].rstrip().endswith(",")


def set_comma(line: str, comma: bool) -> str:
    """A constant's line ending in a comma or not, its comment kept."""
    comment = trailing_comment(line)
    code = line.rstrip("\n")[:len(line.rstrip("\n")) - len(comment)] if comment else line.rstrip("\n")
    stripped = code.rstrip()
    gap = code[len(stripped):]
    return stripped.rstrip(",") + ("," if comma else "") + gap + comment + "\n"


def field_names(body: str) -> set[str]:
    """Names of the fields of a struct or union body (code without
    comments), nested members included."""
    names = set()
    for ch in code_chunks(body):
        m = NESTED.match(ch.code)
        if m:
            names |= field_names(ch.code[ch.code.index("{") + 1:ch.code.rindex("}")])
            names.add(m.group(2))
            continue
        m = FN_PTR.search(ch.code)
        if m:
            names.add(m.group(1))
            continue
        m = DECL.match(ch.code)
        if m:
            names.add(m.group(3))
    names.discard("")
    return names


def struct_fields(texts: list[str]) -> dict[str, set[str]]:
    """Struct or union name -> its field names, for every struct in texts.
    A typedef'd struct appears under its typedef name only."""
    out = {}
    structs = Types(texts).structs
    typedefs = {body for name, (_, body) in structs.items() if not name.startswith("struct ")}
    for name, (_, body) in structs.items():
        if name.startswith("struct ") and body in typedefs:
            continue
        try:
            out[name] = field_names(body)
        except MergeError:
            pass
    return out


def declared_functions(text: str) -> set[str]:
    """Names of the functions a header declares."""
    try:
        return {e.key[1] for e in parse_header(text)[0] if e.key[0] == "func"}
    except MergeError:
        return set(re.findall(r"^\s*[\w\s\*]+?\b(\w+)\s*\([^;{]*\)\s*;", text, re.M))
