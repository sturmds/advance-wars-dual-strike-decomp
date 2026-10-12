#!/usr/bin/env python3
"""Maps in assets/maps/: unpack them from the ROM, and pack them for the build.

    maps.py unpack [--out assets/maps]   ROM (extract/) -> one YAML file per map, plus the glue files
    maps.py pack [--out build/gen/maps]  assets/maps -> C that src/game/ov000/map/ compiles
    maps.py lcf LCF                      define the symbols inside Ov000_gMapHeaders in a linker script

Layout (docs/maps.md has the details):

  assets/maps/maps.txt     map id -> YAML file, in Ov000_gMapHeaders order
  assets/maps/tiles.txt    tile blocks in .rodata order (YAML file + `tiles` or `hardTiles`)
  assets/maps/units.txt    unit lists in .data order (YAML file + `units` or `hardUnits`)
  assets/maps/<group>/NNN_<name>.yaml
      header:    the MapHeader fields by their C names (include/game/map_header.h)
      tiles, hardTiles:  width, height and one row of 3-digit hex tile codes per line
      units, hardUnits:  the PredeployedUnit rows of each army

`pack` reads the fields of MapHeader, PredeployedUnit, MapSlotFlags and
TextRef from include/ (c_fields), checks every YAML key against them, and
writes the initializers in declaration order: mwcc 2.0 has no designated
initializers. A word in capitals (CO_ANDY, UNIT_TANK) is a constant from
include/; any other word is a symbol, declared `extern char NAME[]`.
`unpack` reads field offsets from build/dsindex.json (tools/dsindex.py),
which the compiler fills from the same headers.

Tile blocks are LZ77 (type 0x10, the SDK's Unpack_Lz77ByHalfwords format). The
compressor takes the longest match and, among equal ones, the nearest; that
gives the ROM's bytes for all 260 blocks.
"""

import argparse
import functools
import re
import sys
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
ASSETS = Path("assets/maps")
GEN = Path("build/gen/maps")
EXTRACT = Path("extract")
CONFIG = Path("config/AWRE/arm9")
INCLUDE = Path("include")

# MapHeader pointer fields that the tiles, hardTiles, units and hardUnits
# sections stand for.
DATA_FIELDS = {"mapData": "tiles", "hardMapData": "hardTiles", "units": "units", "hardUnits": "hardUnits"}
TILE_KEYS = ("tiles", "hardTiles")
UNIT_KEYS = ("units", "hardUnits")
# Fields written with enum constants: (struct, field) -> (header, enum).
ENUM_FIELDS = {
    ("MapHeader", "co"): ("game/co_data.h", "CoId"),
    ("PredeployedUnit", "type"): ("game/unit_stats.h", "UnitType"),
}
# Directory of each MapHeader.mapCase (enum MapCase).
GROUPS = {0: "campaign", 1: "campaign", 2: "versus", 3: "versus", 4: "versus", 5: "versus", 6: "versus",
          7: "warroom", 8: "warroom", 9: "survival", 10: "other", 0xffff: "other"}
# Structs that map files spell out, and the header that declares each.
STRUCT_HEADERS = {"MapHeader": "game/map_header.h", "MapSlotFlags": "game/map_header.h",
                  "PredeployedUnit": "game/predeployed_unit.h", "TextRef": "game/text_ref.h"}
UNIT_ROW = 13                      # sizeof(PredeployedUnit)
PREDEPLOYED_ARMY = 0xfe
PREDEPLOYED_END = 0xff


# ---------------------------------------------------------------- LZ77

def lz_unpack(data: bytes, offset: int = 0) -> tuple[bytes, int]:
    """(unpacked bytes, packed length) of the LZ77 block at offset."""
    if data[offset] != 0x10:
        raise ValueError(f"no LZ77 block at {offset:#x}")
    size = int.from_bytes(data[offset + 1:offset + 4], "little")
    out = bytearray()
    p = offset + 4
    while len(out) < size:
        flags = data[p]
        p += 1
        for bit in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> bit):
                pair = data[p] << 8 | data[p + 1]
                p += 2
                back = (pair & 0xfff) + 1
                for _ in range((pair >> 12) + 3):
                    out.append(out[-back])
            else:
                out.append(data[p])
                p += 1
    return bytes(out), p - offset


def lz_pack(data: bytes) -> bytes:
    """LZ77 with the longest match within 0x1000 bytes, the nearest of equal ones."""
    out = bytearray([0x10]) + len(data).to_bytes(3, "little")
    seen: dict[bytes, list[int]] = {}
    i = 0

    def remember(upto: int) -> None:
        nonlocal done
        while done < upto:
            if done + 3 <= len(data):
                seen.setdefault(data[done:done + 3], []).append(done)
            done += 1

    done = 0
    while i < len(data):
        flag_at = len(out)
        out.append(0)
        for bit in range(8):
            if i >= len(data):
                break
            remember(i)
            best, best_back = 0, 0
            for j in reversed(seen.get(data[i:i + 3], [])):
                if i - j > 0x1000:
                    break
                n = 0
                while n < 18 and i + n < len(data) and data[j + n] == data[i + n]:
                    n += 1
                if n > best:
                    best, best_back = n, i - j
                    if n == 18:
                        break
            if best >= 3:
                out[flag_at] |= 0x80 >> bit
                pair = (best - 3) << 12 | (best_back - 1)
                out += bytes([pair >> 8, pair & 0xff])
                i += best
            else:
                out.append(data[i])
                i += 1
    return bytes(out)


def pad4(data: bytes) -> bytes:
    return data + bytes(-len(data) % 4)


# ---------------------------------------------------------------- headers in include/

def enum_values(header: str, name: str) -> dict[str, int]:
    """Constants of `enum name` in include/header: name -> value."""
    text = (ROOT / INCLUDE / header).read_text()
    body = re.search(rf"enum {name}\s*\{{(.*?)\}};", text, re.S)
    if not body:
        sys.exit(f"error: no enum {name} in include/{header}")
    values: dict[str, int] = {}
    nxt = 0
    for item in re.sub(r"/\*.*?\*/", "", body.group(1), flags=re.S).split(","):
        item = item.strip()
        if not item:
            continue
        key, _, expr = item.partition("=")
        key, expr = key.strip(), expr.strip()
        if expr:
            nxt = values[expr] if expr in values else int(expr, 0)
        values[key] = nxt
        nxt += 1
    return values


def enum_names(header: str, name: str) -> dict[int, str]:
    """value -> constant, the first name of each value, skipping *_COUNT."""
    names: dict[int, str] = {}
    for key, value in enum_values(header, name).items():
        if not key.endswith("_COUNT"):
            names.setdefault(value, key)
    return names


def define(header: str, name: str) -> int:
    m = re.search(rf"#define {name}\s+(\S+)", (ROOT / INCLUDE / header).read_text())
    if not m:
        sys.exit(f"error: no #define {name} in include/{header}")
    return int(m.group(1), 0)


def statements(body: str) -> list[str]:
    """The `;`-separated declarations of a struct body, nested braces kept whole."""
    out, depth, start = [], 0, 0
    for i, ch in enumerate(body):
        depth += {"{": 1, "}": -1}.get(ch, 0)
        if ch == ";" and depth == 0:
            out.append(body[start:i].strip())
            start = i + 1
    return [s for s in out if s]


@functools.cache
def c_fields(name: str) -> list[tuple[str, str, list[int]]]:
    """(field, type, array sizes) of a struct in include/, in declaration
    order. For a union, the fields of its first member: those are what an
    initializer fills."""
    text = re.sub(r"/\*.*?\*/", "", (ROOT / INCLUDE / STRUCT_HEADERS[name]).read_text(), flags=re.S)
    m = re.search(rf"typedef (struct|union) {name} \{{", text)
    if not m:
        sys.exit(f"error: no typedef {name} in include/{STRUCT_HEADERS[name]}")
    depth, i = 1, m.end()
    while depth:
        depth += {"{": 1, "}": -1}.get(text[i], 0)
        i += 1
    decls = statements(text[m.end():i - 1])
    if m.group(1) == "union":
        inner = re.fullmatch(r"struct\s*\{(.*)\}", decls[0], re.S)
        decls = statements(inner.group(1)) if inner else decls[:1]
    fields = []
    for decl in decls:
        d = re.fullmatch(r"(.*?)(\w+)\s*((?:\[[^\]]*\]\s*)*)", decl, re.S)
        sizes = [int(eval(s, {})) for s in re.findall(r"\[([0-9a-fA-Fx +\-*]+)\]", d.group(3))]
        fields.append((d.group(2), " ".join(d.group(1).split()), sizes))
    return fields


def layouts() -> dict[str, dict]:
    """Struct layouts from build/dsindex.json, by name."""
    sys.path.insert(0, str(ROOT / "tools"))
    import dsindex
    out = {}
    for rec in dsindex.load()["headers"].values():
        for s in rec.get("structs", []):
            out.setdefault(s["name"], s)
    return out


# ---------------------------------------------------------------- ROM side (unpack)

class Rom:
    """ov000 bytes, symbols of every ARM9 module, and ov000's relocations."""

    def __init__(self) -> None:
        info = yaml.safe_load((ROOT / EXTRACT / "arm9_overlays/overlays.yaml").read_text())
        ov0 = next(o for o in info["overlays"] if o["id"] == 0)
        self.base = ov0["base_address"]
        self.data = (ROOT / EXTRACT / "arm9_overlays" / ov0["file_name"]).read_bytes()
        self.symbols: dict[str, dict[int, str]] = {}
        for path in sorted((ROOT / CONFIG).rglob("symbols.txt")):
            module = "main" if path.parent == ROOT / CONFIG else path.parent.name
            table = self.symbols.setdefault(module, {})
            for line in path.read_text().splitlines():
                m = re.match(r"(\S+) kind:\S+ addr:(0x[0-9a-f]+)", line)
                if m:
                    table.setdefault(int(m.group(2), 16), m.group(1))
        self.relocs: dict[int, tuple[int, str]] = {}
        for line in (ROOT / CONFIG / "overlays/ov000/relocs.txt").read_text().splitlines():
            m = re.match(r"from:(0x\w+) kind:\S+ to:(0x\w+) module:(\S+)", line)
            if m:
                self.relocs[int(m.group(1), 16)] = (int(m.group(2), 16), m.group(3))

    def symbol(self, name: str) -> int:
        return next(a for a, n in self.symbols["ov000"].items() if n == name)

    def read(self, addr: int, size: int) -> bytes:
        return self.data[addr - self.base:addr - self.base + size]

    def word(self, addr: int, size: int = 4) -> int:
        return int.from_bytes(self.read(addr, size), "little")

    def pointer_name(self, addr: int) -> str | None:
        """Symbol that the pointer at addr points to; None for NULL."""
        if self.word(addr) == 0:
            return None
        to, module = self.relocs[addr]
        m = re.match(r"overlays?\((\d+)", module)
        key = f"ov{int(m.group(1)):03d}" if m else module
        try:
            return self.symbols[key][to]
        except KeyError:
            sys.exit(f"error: no symbol at {to:#x} in {key} (pointer at {addr:#x})")

    def text(self, text_id: int) -> str:
        """A game string: the bank whose first id is at or below text_id (Text_Get)."""
        bank = self.symbol("gTextBanks")
        while self.word(bank + 4):
            if text_id >= self.word(bank):
                start = self.word(self.word(bank + 4) + 4 * (text_id & 0xffffff)) - self.base
                return self.data[start:self.data.index(0, start)].decode("latin-1")
            bank += 8
        return ""


def dims(type_name: str) -> list[int]:
    return [int(n, 0) for n in re.findall(r"\[(\w+)\]", type_name)]


class Decoder:
    """Turns bytes at an address into YAML values, field by field."""

    def __init__(self, rom: Rom, structs: dict[str, dict]) -> None:
        self.rom = rom
        self.structs = structs
        self.enums = {key: enum_names(*where) for key, where in ENUM_FIELDS.items()}

    def struct(self, name: str, addr: int, skip: set[str] = frozenset()) -> dict:
        out = {}
        for f in self.structs[name]["fields"]:
            if f["name"] in skip or (f["name"].startswith("pad_") and not any(self.rom.read(addr + f["offset"], f["size"]))):
                continue
            out[f["name"]] = self.field(name, f, addr + f["offset"])
        return out

    def field(self, owner: str, f: dict, addr: int):
        shape = dims(f["type"])
        base_type = re.sub(r"\[.*", "", f["type"]).strip()
        elem = f["size"]
        for n in shape:
            elem //= n
        enum = self.enums.get((owner, f["name"]))

        def one(a: int):
            if base_type.endswith("*"):
                return self.rom.pointer_name(a)
            if "struct" in f:
                s = self.structs[f["struct"]]
                if s["kind"] == "union":
                    members = {m["name"]: m for m in s["fields"]}
                    return {n: Hex(self.rom.word(a + members[n]["offset"], members[n]["size"]))
                            for n, _, _ in c_fields(f["struct"])}
                return self.struct(f["struct"], a)
            value = self.rom.word(a, elem)
            return enum.get(value, value) if enum else value

        def nest(a: int, rest: list[int]):
            if not rest:
                return one(a)
            step = elem
            for n in rest[1:]:
                step *= n
            return [nest(a + i * step, rest[1:]) for i in range(rest[0])]

        return nest(addr, shape)


class Hex(int):
    """An int that unpack writes in hex."""


def tile_rows(raw: bytes) -> dict:
    width, height = raw[0], raw[1]
    codes = [int.from_bytes(raw[2 + 2 * i:4 + 2 * i], "little") for i in range(width * height)]
    rows = [" ".join(f"{c:03x}" for c in codes[y * width:(y + 1) * width]) for y in range(height)]
    return {"width": width, "height": height, "rows": rows}


def unit_armies(rom: Rom, decoder: Decoder, addr: int) -> tuple[list[dict], int]:
    """(armies, address after the end row) of the unit list at addr."""
    armies: list[dict] = []
    while True:
        row = rom.read(addr, UNIT_ROW)
        if row[0] == PREDEPLOYED_END:
            if any(row[1:]):
                sys.exit(f"error: end row at {addr:#x} is not ff 00..")
            return armies, addr + UNIT_ROW
        if row[0] == PREDEPLOYED_ARMY:
            if any(row[2:]):
                sys.exit(f"error: army row at {addr:#x} has more than an army number")
            armies.append({"army": row[1], "rows": []})
        else:
            if not armies:
                sys.exit(f"error: unit row at {addr:#x} before any army row")
            armies[-1]["rows"].append(decoder.struct("PredeployedUnit", addr))
        addr += UNIT_ROW


def slug(name: str) -> str:
    return re.sub(r"[^a-z0-9]+", "_", name.lower().replace("'", "")).strip("_")


# ---------------------------------------------------------------- YAML writing

def scalar(v) -> str:
    if v is None:
        return "null"
    if isinstance(v, Hex):
        return f"{int(v):#x}"
    return str(v)


def flow(v) -> str:
    if isinstance(v, list):
        return "[" + ", ".join(flow(x) for x in v) + "]"
    if isinstance(v, dict):
        return "{" + ", ".join(f"{k}: {flow(x)}" for k, x in v.items()) + "}"
    return scalar(v)


def write_map(path: Path, title: str, map_id: int, doc: dict) -> None:
    lines = [f"# {title} (map {map_id:#05x})", "header:"]
    for key, value in doc["header"].items():
        lines.append(f"  {key}: {flow(value)}")
    for key in TILE_KEYS:
        if key in doc:
            t = doc[key]
            lines += [f"{key}:", f"  width: {t['width']}", f"  height: {t['height']}", "  rows:"]
            lines += [f"  - {row}" for row in t["rows"]]
    for key in UNIT_KEYS:
        if key in doc:
            lines.append(f"{key}:" + ("" if doc[key] else " []"))
            for army in doc[key]:
                lines.append(f"- army: {army['army']}")
                lines.append("  rows:" + (" []" if not army["rows"] else ""))
                lines += [f"  - {flow(r)}" for r in army["rows"]]
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n")


def cmd_unpack(args) -> int:
    rom = Rom()
    structs = layouts()
    decoder = Decoder(rom, structs)
    table = rom.symbol("Ov000_gMapHeaders")
    size = structs["MapHeader"]["size"]
    count = define("game/map_header.h", "MAP_COUNT")
    offsets = {f["name"]: f["offset"] for f in structs["MapHeader"]["fields"]}
    out = ROOT / args.out
    paths: list[str] = []
    tile_blocks: list[tuple[int, str, str]] = []
    unit_lists: list[tuple[int, str, str]] = []
    for map_id in range(count):
        entry = table + map_id * size
        header = decoder.struct("MapHeader", entry, skip=set(DATA_FIELDS))
        title = rom.text(header["name"]["group"] << 16 | header["name"]["index"])
        case = header["mapCase"]
        name = "unused" if case == 0xffff else slug(title)
        rel = f"{GROUPS[case]}/{map_id:03x}_{name}.yaml"
        doc: dict = {"header": header}
        for field, key in DATA_FIELDS.items():
            addr = rom.word(entry + offsets[field])
            if not addr:
                continue
            if key in TILE_KEYS:
                raw, _ = lz_unpack(rom.data, addr - rom.base)
                doc[key] = tile_rows(raw)
                tile_blocks.append((addr, rel, key))
            else:
                doc[key], _ = unit_armies(rom, decoder, addr)
                unit_lists.append((addr, rel, key))
        write_map(out / rel, title, map_id, doc)
        paths.append(rel)
    (out / "maps.txt").write_text(
        "# Map header table (Ov000_gMapHeaders): one map id and YAML file per line, in id order.\n"
        + "".join(f"{i:#05x} {p}\n" for i, p in enumerate(paths)))
    (out / "tiles.txt").write_text(
        "# Tile blocks in ov000 .rodata order: YAML file and section.\n"
        + "".join(f"{p} {k}\n" for _, p, k in sorted(tile_blocks)))
    (out / "units.txt").write_text(
        "# Pre-deployed unit lists in ov000 .data order: YAML file and section.\n"
        + "".join(f"{p} {k}\n" for _, p, k in sorted(unit_lists)))
    print(f"unpack: {len(paths)} maps, {len(tile_blocks)} tile blocks, {len(unit_lists)} unit lists in {args.out}")
    return 0


# ---------------------------------------------------------------- build side (pack)

def glue(path: Path) -> list[list[str]]:
    lines = []
    for n, line in enumerate(path.read_text().splitlines(), 1):
        line = line.split("#", 1)[0].strip()
        if line:
            lines.append(line.split() + [f"{path.name}:{n}"])
    return lines


def c_scalar(v, type_name: str, where: str, externs: set[str]) -> str:
    if "*" in type_name:
        if v is None:
            return "NULL"
        if isinstance(v, str) and re.fullmatch(r"[A-Za-z_]\w*", v) and not v.isupper():
            externs.add(v)
            return f"(void *){v}"
        sys.exit(f"error: {where}: {v!r} is not a symbol or null")
    if isinstance(v, int) and not isinstance(v, bool):
        return f"{v:#x}" if v > 9 else str(v)
    if isinstance(v, str) and re.fullmatch(r"[A-Z_][A-Z0-9_]*", v):
        return v
    sys.exit(f"error: {where}: {v!r} is not a number or a constant")


def c_init(v, type_name: str, sizes: list[int], where: str, externs: set[str]) -> str:
    """A YAML value as the C initializer of a field of that type."""
    if sizes:
        if not isinstance(v, list) or len(v) != sizes[0]:
            sys.exit(f"error: {where}: expected a list of {sizes[0]}")
        return "{ " + ", ".join(c_init(x, type_name, sizes[1:], f"{where}[{i}]", externs)
                                for i, x in enumerate(v)) + " }"
    base = type_name.replace("const ", "").strip()
    if base in STRUCT_HEADERS:
        if not isinstance(v, dict):
            sys.exit(f"error: {where}: expected the fields of {base}")
        return c_struct(base, v, where, externs)
    return c_scalar(v, type_name, where, externs)


def c_struct(name: str, v: dict, where: str, externs: set[str], given: dict[str, str] | None = None) -> str:
    """Initializer of struct `name` from a YAML mapping, fields in the order
    include/ declares them. A missing pad_ field is zero; `given` fields are
    written by the caller."""
    fields = c_fields(name)
    given = given or {}
    unknown = set(v) - {f for f, _, _ in fields}
    if unknown:
        sys.exit(f"error: {where}: {name} has no field {', '.join(sorted(unknown))}")
    parts = []
    for field, type_name, sizes in fields:
        if field in given:
            if field in v:
                sys.exit(f"error: {where}: {field} comes from its {DATA_FIELDS[field]} section")
            parts.append(given[field])
        elif field in v:
            parts.append(c_init(v[field], type_name, sizes, f"{where}.{field}", externs))
        elif field.startswith("pad_"):
            parts.append("{ 0 }" if sizes else "0")
        else:
            sys.exit(f"error: {where}: no {field}")
    return "{ " + ", ".join(parts) + " }"


def tile_bytes(t: dict, where: str) -> bytes:
    width, height, rows = t["width"], t["height"], t["rows"]
    if len(rows) != height:
        sys.exit(f"error: {where}: {len(rows)} rows, height {height}")
    raw = bytearray([width, height])
    for y, row in enumerate(rows):
        codes = str(row).split()
        if len(codes) != width:
            sys.exit(f"error: {where}: row {y} has {len(codes)} codes, width {width}")
        for c in codes:
            raw += int(c, 16).to_bytes(2, "little")
    return bytes(raw)


def unit_rows(armies: list[dict], where: str) -> list[str]:
    rows = []
    for army in armies:
        rows.append(f"{{ PREDEPLOYED_ARMY, {army['army']} }}")
        rows += [c_struct("PredeployedUnit", r, f"{where} army {army['army']}", set()) for r in army["rows"]]
    rows.append("{ PREDEPLOYED_END }")
    return rows


def write_if_changed(path: Path, text: str) -> None:
    if not path.exists() or path.read_text() != text:
        path.write_text(text)


def cmd_pack(args) -> int:
    assets = ROOT / args.assets
    loader = getattr(yaml, "CSafeLoader", yaml.SafeLoader)
    maps: dict[str, dict] = {}
    order: list[tuple[int, str]] = []
    for map_id, rel, where in glue(assets / "maps.txt"):
        if int(map_id, 0) != len(order):
            sys.exit(f"error: {where}: map id {map_id}, expected {len(order):#05x}")
        maps[rel] = yaml.load((assets / rel).read_text(), Loader=loader)
        order.append((int(map_id, 0), rel))

    def member(rel: str, key: str) -> str:
        return f"m{rel.split('/')[-1][:3]}" + ("_hard" if key.startswith("hard") else "")

    def placed(name: str, keys: tuple[str, str]) -> dict[tuple[str, str], str]:
        found: dict[tuple[str, str], str] = {}
        for rel, key, where in glue(assets / name):
            if rel not in maps:
                sys.exit(f"error: {where}: {rel} is not in maps.txt")
            if key not in keys or key not in maps[rel]:
                sys.exit(f"error: {where}: {rel} has no {key} section")
            if (rel, key) in found:
                sys.exit(f"error: {where}: {rel} {key} is listed twice")
            found[(rel, key)] = where
        for rel, doc in maps.items():
            for key in keys:
                if key in doc and (rel, key) not in found:
                    sys.exit(f"error: {rel} has a {key} section that {name} does not list")
        return found

    tiles = placed("tiles.txt", TILE_KEYS)
    units = placed("units.txt", UNIT_KEYS)
    generated_note = "/* Generated by tools/maps.py pack from assets/maps/. Do not edit. */\n"

    h = [generated_note, "#ifndef MAPS_MAP_DATA_H", "#define MAPS_MAP_DATA_H", "",
         '#include "types.h"', '#include "game/predeployed_unit.h"', "",
         "/* The tile blocks of every map in assets/maps/tiles.txt order: LZ77, padded to 4 bytes. */",
         "typedef struct MapTileData {"]
    t_init = [generated_note, "const MapTileData gMapTiles = {"]
    for rel, key in tiles:
        packed = pad4(lz_pack(tile_bytes(maps[rel][key], f"{rel} {key}")))
        h.append(f"    u8 {member(rel, key)}[{len(packed):#x}];".ljust(39) + f"/* {rel} {key} */")
        body = ", ".join(f"{b:#04x}" for b in packed)
        t_init.append(f"    {{ {body} }},")
    t_init.append("};")
    h += ["} MapTileData;", "",
          "/* The pre-deployed unit lists in assets/maps/units.txt order, each padded to 4 bytes. */",
          "typedef struct MapUnitData {"]
    u_init = [generated_note, "MapUnitData gMapUnits = {"]
    for rel, key in units:
        rows = unit_rows(maps[rel][key], f"{rel} {key}")
        h.append(f"    PredeployedUnit {member(rel, key)}[{len(rows)}];".ljust(39) + f"/* {rel} {key} */")
        u_init.append("    {")
        u_init += [f"        {r}," for r in rows]
        u_init.append("    },")
        pad = -len(rows) * UNIT_ROW % 4
        if pad:
            h.append(f"    u8 {member(rel, key)}_pad[{pad}];")
            u_init.append("    { 0 },")
    u_init.append("};")
    h += ["} MapUnitData;", "", "extern const MapTileData gMapTiles;", "extern MapUnitData gMapUnits;", "",
          "#endif"]

    externs: set[str] = set()
    entries = []
    for map_id, rel in order:
        doc = maps[rel]
        given = {}
        for field, key in DATA_FIELDS.items():
            table = "gMapTiles" if key in TILE_KEYS else "gMapUnits"
            given[field] = f"{table}.{member(rel, key)}" if key in doc else "NULL"
        entries.append(f"    /* {map_id:#05x} {rel} */")
        entries.append(f"    {c_struct('MapHeader', doc['header'], rel, externs, given)},")
    m_init = [generated_note]
    m_init += [f"extern char {name}[];" for name in sorted(externs)]
    m_init += ["", f"typedef char MapHeaders_count_check[{len(order)} == MAP_COUNT ? 1 : -1];", "",
               f"MapHeader Ov000_gMapHeaders[{len(order)}] = {{", *entries, "};"]

    out = ROOT / args.out
    out.mkdir(parents=True, exist_ok=True)
    write_if_changed(out / "map_data.h", "\n".join(h) + "\n")
    write_if_changed(out / "map_tiles.inc", "\n".join(t_init) + "\n")
    write_if_changed(out / "map_units.inc", "\n".join(u_init) + "\n")
    write_if_changed(out / "map_headers.inc", "\n".join(m_init) + "\n")
    return 0


# ---------------------------------------------------------------- linker script

def header_aliases() -> dict[str, int]:
    """Symbols inside Ov000_gMapHeaders that other code uses (field bases such
    as data_ov000_022dbd50), with their offset into the table. The linker
    script defines them (cmd_lcf), because a C array cannot."""
    syms = {}
    for line in (ROOT / CONFIG / "overlays/ov000/symbols.txt").read_text().splitlines():
        m = re.match(r"(\S+) kind:\S+ addr:(0x[0-9a-f]+)", line)
        if m:
            syms[m.group(1)] = int(m.group(2), 16)
    delinks = (ROOT / CONFIG / "overlays/ov000/delinks.txt").read_text()
    m = re.search(r"^src/game/ov000/map/map_headers\.c:\n(?:    .*\n)*?    \.data\s+start:(0x\w+) end:(0x\w+)",
                  delinks, re.M)
    start, end = int(m.group(1), 16), int(m.group(2), 16)
    if syms["Ov000_gMapHeaders"] != start:
        sys.exit("error: Ov000_gMapHeaders is not where map_headers.c starts")
    return {name: addr - start for name, addr in syms.items() if start < addr < end}


def cmd_lcf(args) -> int:
    """Writes `NAME = Ov000_gMapHeaders + OFFSET;` after the line that places
    map_headers.o: before that line the linker takes the table's address as 0."""
    path = Path(args.lcf)
    text = path.read_text()
    anchor = "        map_headers.o(.data)\n"
    if text.count(anchor) != 1:
        sys.exit(f"error: {anchor.strip()} is not in {path} exactly once")
    lines = "".join(f"        {name} = Ov000_gMapHeaders + {offset:#x};\n"
                    for name, offset in sorted(header_aliases().items()))
    if lines not in text:
        path.write_text(text.replace(anchor, anchor + lines))
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(required=True)
    p = sub.add_parser("unpack", help="write assets/maps/ from extract/")
    p.add_argument("--out", default=str(ASSETS))
    p.set_defaults(func=cmd_unpack)
    p = sub.add_parser("pack", help="write the C for the map tables")
    p.add_argument("--assets", default=str(ASSETS))
    p.add_argument("--out", default=str(GEN))
    p.set_defaults(func=cmd_pack)
    p = sub.add_parser("lcf", help="define the symbols inside Ov000_gMapHeaders in a linker script")
    p.add_argument("lcf")
    p.set_defaults(func=cmd_lcf)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
