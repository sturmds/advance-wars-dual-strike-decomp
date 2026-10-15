#!/usr/bin/env python3
"""Graphics: unpack tile data from the ROM to indexed PNG files, pack them back.

    gfx.py unpack                                every picture file of the ROM -> extracted/AWRE/assets/gfx/files/
    gfx.py unpack FILE [--out DIR] [--layout NAME] [--palette SRC]   one tile file -> NAME.png + NAME.yaml
    gfx.py pack YAML [--out FILE] [--palette-out FILE]   the PNG beside YAML -> the file's bytes (compressed as before)
    gfx.py check [--dir extract/files/syogun]    unpack and pack every file of a directory; compare bytes
    gfx.py co CO [--out DIR] [--palette N]       every picture of one CO (name or id), from gCoGraphics
    gfx.py layouts                               the layouts unpack knows

The game keeps its pictures as 4-bit tiles (32 bytes per 8x8 tile, two
pixels a byte, low nibble first), mostly LZ77-compressed (type 0x10, the
BIOS format), with 16-colour BGR555 palettes in main's data. There are no
Nitro NCGR/NCLR files in this ROM. See docs/gfx.md.

A layout says how the tiles make a picture: the sprite pieces (OBJ shapes)
the game draws them with, filled in tile order with 1D mapping, or a plain
grid N tiles wide (`grid:N`). `pack` reads the layout and the compression
from the YAML file, so it writes the same bytes as the ROM for an unchanged
PNG. Colour 0 of each palette is transparent; the PNG marks it so.

The PNG and YAML files hold game data from the builder's ROM: they belong in
extracted/ or build/ and never enter git (tenet 9). `make setup` runs
`unpack` with no file, which writes every picture file of the ROM (LZ77 tile
data, not the res_* containers) under its ROM path:

  extracted/AWRE/assets/gfx/files/syogun/076.png  .yaml

The build packs each of those into build/rom/files/ (one ninja step per file,
`pack --out`), and tools/rom_files.py makes that directory the ROM's file
system: an edited PNG changes build/AWRE.nds, and a new PNG and YAML file adds
a file to the ROM. See docs/gfx.md, "Build".

`co` writes a view by CO, for looking at; the build does not read it:

  extracted/AWRE/assets/gfx/co/NN_NAME/
      full_v0.png              CO picture, top and bottom pieces joined (144x320)
      full_v0_top.png  .yaml   /syogun file of pictures[v].name (144x192)
      full_v0_bottom.png .yaml /syogun file of pictures[v].unk_04 (144x128)
      face_v0_m0.png .yaml     dialogue face per mood (48x48)
      battle_v0_m0.png .yaml   battle screen face per mood (40x48)
      medium_v0.png .yaml      Versus slot picture (24x48)
      small_v0.png             12-tile picture in main's data (48x16)
      hud_v0.png               map panel face in main's data (32x16)
      palettes.png             the CO's 8 palettes, one row each
"""

import argparse
import hashlib
import re
import struct
import sys
import tempfile
import zlib
from pathlib import Path

import yaml

from rom_files import write_if_changed

ROOT = Path(__file__).resolve().parent.parent
FILES = Path("extract/files")
ARM9 = Path("extract/arm9/arm9.bin")
ARM9_BASE = 0x02000000
SYMBOLS = Path("config/AWRE/arm9/symbols.txt")
CO_HEADER = Path("include/game/co_data.h")
EXTRACTED = Path("extracted/AWRE/assets/gfx")  # gitignored
PICTURES = EXTRACTED / "files"  # by ROM path: the files the build packs
UNPACKED = ".unpacked"  # written last by `unpack` with no file; tools/wt_setup.sh copies only a whole unpack

TILE = 8
LZ77 = 0x10

# ---------------------------------------------------------------------------
# LZ77 (type 0x10)
#
# Header: byte 0x10, then the decompressed size in 3 bytes. Then groups of a
# flag byte and 8 items, high bit first: 0 a literal byte, 1 a 2-byte
# reference (4 bits length - 3, 12 bits distance - 1). The compressor the
# game's files came from is greedy, takes the nearest of the longest matches
# and never uses distance 1 (so the BIOS can unpack straight to VRAM, which
# takes 16-bit writes). The output is padded with zeros to 4 bytes.


def lz77_unpack(data: bytes) -> tuple[bytes, int]:
    """The decompressed bytes and the length of the compressed stream, padding included."""
    if len(data) < 4 or data[0] != LZ77:
        raise ValueError("not LZ77 type 0x10")
    size = int.from_bytes(data[1:4], "little")
    out = bytearray()
    i = 4
    while len(out) < size:
        flags = data[i]
        i += 1
        for bit in range(8):
            if len(out) >= size:
                break
            if flags & (0x80 >> bit):
                ref = data[i] << 8 | data[i + 1]
                i += 2
                length = (ref >> 12) + 3
                dist = (ref & 0xFFF) + 1
                if dist > len(out):
                    raise ValueError("reference before the start")
                for _ in range(length):
                    out.append(out[-dist])
            else:
                out.append(data[i])
                i += 1
    return bytes(out), (i + 3) & ~3


def lz77_pack(data: bytes) -> bytes:
    """Compress the way the game's files were: greedy, nearest longest match, distance 2 to 4096."""
    n = len(data)
    out = bytearray([LZ77]) + n.to_bytes(3, "little")
    chains: dict[bytes, list[int]] = {}  # 3-byte prefix -> positions, ascending
    added = 0  # positions below this are in chains

    i = 0
    while i < n:
        flag_at = len(out)
        out.append(0)
        flags = 0
        for bit in range(8):
            if i >= n:
                break
            while added <= i - 2:  # distance 1 is never used
                if added + 3 <= n:
                    chains.setdefault(data[added:added + 3], []).append(added)
                added += 1
            best_len, best_dist = 0, 0
            limit = min(18, n - i)
            if limit >= 3:
                for j in reversed(chains.get(data[i:i + 3], ())):
                    if i - j > 0x1000:
                        break
                    length = 3
                    while length < limit and data[j + length] == data[i + length]:
                        length += 1
                    if length > best_len:
                        best_len, best_dist = length, i - j
                        if length == limit:
                            break
            if best_len >= 3:
                flags |= 0x80 >> bit
                ref = (best_len - 3) << 12 | (best_dist - 1)
                out += bytes((ref >> 8, ref & 0xFF))
                i += best_len
            else:
                out.append(data[i])
                i += 1
        out[flag_at] = flags
    while len(out) % 4:
        out.append(0)
    return bytes(out)


# ---------------------------------------------------------------------------
# Layouts: sprite pieces (x, y, width, height) in pixels, filled in order.


def grid(width_tiles: int, tiles: int) -> list[tuple[int, int, int, int]]:
    if tiles % width_tiles:
        raise ValueError(f"{tiles} tiles do not fill rows of {width_tiles}")
    return [(0, 0, width_tiles * TILE, tiles // width_tiles * TILE)]


def strips(x: int, w: int, h: int, count: int) -> list[tuple[int, int, int, int]]:
    """count pieces of w x h stacked down from (x, 0)."""
    return [(x, k * h, w, h) for k in range(count)]


LAYOUTS = {
    # Co_LoadGraphics, gCoPortraitSprite: six 64x64 pieces in rows of two,
    # then six 16x32 pieces down the right edge.
    "co_full_top": [(x, y, 64, 64) for y in (0, 64, 128) for x in (0, 64)] + strips(128, 16, 32, 6),
    # Co_LoadObjPicture: the legs, drawn 192 pixels below the top piece
    # (TagBreak_DrawSidePiece). Same scheme, two rows.
    "co_full_bottom": [(x, y, 64, 64) for y in (0, 64) for x in (0, 64)] + strips(128, 16, 32, 4),
    # Co_LoadMoodPicture and friends, data_02151df8: rows of 32x8 + 16x8.
    "co_face": [(x, y, w, 8) for y in range(0, 48, 8) for x, w in ((0, 32), (32, 16))],
    # Co_LoadMoodPicture into the battle screen BG: a grid 5 tiles wide.
    "co_battle": [(0, 0, 40, 48)],
    # Co_QueueArmyMediumPicture, gCoPictureSprite.
    "co_medium": [(0, 0, 16, 32), (16, 0, 8, 32), (0, 32, 16, 16), (16, 32, 8, 16)],
    # Co_LoadSmallPicture, data_02151ccc.
    "co_small": [(0, 0, 32, 16), (32, 0, 16, 16)],
    # CoPanel_Draw, data_ov000_0234ebf0 (drawn mirrored).
    "co_hud": [(0, 0, 32, 16)],
}

# Layout picked for a file of this many bytes (4 bits per pixel) when none is given.
LAYOUT_BY_SIZE = {
    0x3600: "co_full_top",
    0x2400: "co_full_bottom",
    0x480: "co_face",
    0x3C0: "co_battle",
    0x240: "co_medium",
    0x180: "co_small",
    0x100: "co_hud",
}


def layout_pieces(name: str, size: int, bpp: int) -> list[tuple[int, int, int, int]]:
    tile_bytes = 8 * bpp
    if size % tile_bytes:
        raise ValueError(f"{size} bytes is not a whole number of {bpp}-bit tiles")
    tiles = size // tile_bytes
    if name.startswith("grid:"):
        return grid(int(name[5:]), tiles)
    pieces = LAYOUTS[name]
    need = sum(w * h for _, _, w, h in pieces) // 64
    if need != tiles:
        raise ValueError(f"layout {name} takes {need} tiles, the data has {tiles}")
    return pieces


def default_layout(size: int) -> str:
    if size in LAYOUT_BY_SIZE:
        return LAYOUT_BY_SIZE[size]
    tiles = size // 32
    return f"grid:{next(w for w in (16, 8, 4, 2, 1) if tiles % w == 0)}"


def tiles_to_pixels(data: bytes, pieces, bpp: int) -> tuple[int, int, list[bytearray]]:
    width = max(x + w for x, _, w, _ in pieces)
    height = max(y + h for _, y, _, h in pieces)
    rows = [bytearray(width) for _ in range(height)]
    tile_bytes = 8 * bpp
    pos = 0
    for px, py, pw, ph in pieces:
        for ty in range(0, ph, TILE):
            for tx in range(0, pw, TILE):
                tile = data[pos:pos + tile_bytes]
                pos += tile_bytes
                for y in range(TILE):
                    row = rows[py + ty + y]
                    x0 = px + tx
                    if bpp == 4:
                        for k in range(4):
                            b = tile[y * 4 + k]
                            row[x0 + 2 * k] = b & 0xF
                            row[x0 + 2 * k + 1] = b >> 4
                    else:
                        row[x0:x0 + 8] = tile[y * 8:y * 8 + 8]
    return width, height, rows


def pixels_to_tiles(rows: list[bytes], pieces, bpp: int) -> bytes:
    out = bytearray()
    for px, py, pw, ph in pieces:
        for ty in range(0, ph, TILE):
            for tx in range(0, pw, TILE):
                for y in range(TILE):
                    seg = rows[py + ty + y][px + tx:px + tx + TILE]
                    if bpp == 4:
                        if max(seg) > 15:
                            raise ValueError(f"colour index {max(seg)} at ({px + tx}, {py + ty + y}): 4-bit tiles take 0-15")
                        out += bytes(seg[2 * k] | seg[2 * k + 1] << 4 for k in range(4))
                    else:
                        out += bytes(seg)
    return bytes(out)


# ---------------------------------------------------------------------------
# Palettes: BGR555, 16 colours per 4-bit palette.


def bgr555_to_rgb(colours: bytes) -> list[tuple[int, int, int]]:
    out = []
    for (c,) in struct.iter_unpack("<H", colours):
        r, g, b = c & 31, c >> 5 & 31, c >> 10 & 31
        out.append((r << 3 | r >> 2, g << 3 | g >> 2, b << 3 | b >> 2))
    return out


def rgb_to_bgr555(colours: list[tuple[int, int, int]]) -> bytes:
    return b"".join(struct.pack("<H", r >> 3 | (g >> 3) << 5 | (b >> 3) << 10) for r, g, b in colours)


def grey_palette(count: int) -> list[tuple[int, int, int]]:
    return [(v, v, v) for v in (k * 255 // (count - 1) for k in range(count))]


# ---------------------------------------------------------------------------
# Indexed PNG, written and read with zlib alone so the tool needs no image
# library. Read: colour type 3 (palette), any bit depth, any filter, no
# interlacing; enough for files from GIMP, Aseprite or Pillow.


def png_chunk(kind: bytes, body: bytes) -> bytes:
    return struct.pack(">I", len(body)) + kind + body + struct.pack(">I", zlib.crc32(kind + body))


def write_png(path: Path, rows: list[bytes], palette: list[tuple[int, int, int]], transparent0: bool = True):
    width, height = len(rows[0]), len(rows)
    raw = b"".join(b"\0" + bytes(r) for r in rows)
    png = b"\x89PNG\r\n\x1a\n"
    png += png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 3, 0, 0, 0))
    png += png_chunk(b"PLTE", b"".join(bytes(c) for c in palette))
    if transparent0:
        png += png_chunk(b"tRNS", b"\0")
    png += png_chunk(b"IDAT", zlib.compress(raw, 9))
    png += png_chunk(b"IEND", b"")
    write_if_changed(path, png)


def read_png(path: Path) -> tuple[list[bytearray], list[tuple[int, int, int]]]:
    data = path.read_bytes()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{path}: not a PNG file")
    pos, idat, palette, header = 8, b"", [], None
    while pos < len(data):
        (length,) = struct.unpack_from(">I", data, pos)
        kind, body = data[pos + 4:pos + 8], data[pos + 8:pos + 8 + length]
        pos += 12 + length
        if kind == b"IHDR":
            header = struct.unpack(">IIBBBBB", body)
        elif kind == b"PLTE":
            palette = [tuple(body[k:k + 3]) for k in range(0, len(body), 3)]
        elif kind == b"IDAT":
            idat += body
    width, height, depth, colour_type, _, _, interlace = header
    if colour_type != 3 or interlace:
        raise ValueError(f"{path}: needs an indexed (palette) PNG without interlacing; "
                         "save it with an indexed colour mode of 16 colours")
    raw = zlib.decompress(idat)
    stride = (width * depth + 7) // 8
    bpp = 1  # filter unit: bytes per pixel, at least 1
    rows, prev, pos = [], bytearray(stride), 0
    for _ in range(height):
        kind, line = raw[pos], bytearray(raw[pos + 1:pos + 1 + stride])
        pos += 1 + stride
        for k in range(stride):
            a = line[k - bpp] if k >= bpp else 0
            b = prev[k]
            c = prev[k - bpp] if k >= bpp else 0
            if kind == 1:
                line[k] = (line[k] + a) & 0xFF
            elif kind == 2:
                line[k] = (line[k] + b) & 0xFF
            elif kind == 3:
                line[k] = (line[k] + (a + b) // 2) & 0xFF
            elif kind == 4:
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                line[k] = (line[k] + (a if pa <= pb and pa <= pc else b if pb <= pc else c)) & 0xFF
        prev = line
        if depth == 8:
            rows.append(line)
        else:
            per = 8 // depth
            mask = (1 << depth) - 1
            rows.append(bytearray((line[x // per] >> (8 - depth * (x % per + 1))) & mask for x in range(width)))
    return rows, palette


# ---------------------------------------------------------------------------
# One file


def describe(data: bytes) -> tuple[bytes, str]:
    """The tile bytes of a file and its compression."""
    if data[:1] == bytes([LZ77]):
        try:
            raw, end = lz77_unpack(data)
            if end == len(data):
                return raw, "lz77"
        except (ValueError, IndexError):
            pass
    return data, "none"


def unpack_bytes(data: bytes, png: Path, *, source: str, layout: str | None = None, bpp: int = 4,
                 palette: list[tuple[int, int, int]] | None = None, palette_from: str | None = None,
                 extra: dict | None = None) -> dict:
    raw, compression = describe(data)
    layout = layout or default_layout(len(raw))
    pieces = layout_pieces(layout, len(raw), bpp)
    width, height, rows = tiles_to_pixels(raw, pieces, bpp)
    write_png(png, rows, palette or grey_palette(1 << bpp))
    meta = {"source": source, "compression": compression, "bpp": bpp, "layout": layout,
            "width": width, "height": height, "size": len(raw),
            "sha1": hashlib.sha1(data).hexdigest()}
    if palette_from:
        meta["palette"] = palette_from
    meta.update(extra or {})
    write_if_changed(yaml_of(png), yaml.safe_dump(meta, sort_keys=False).encode())
    return meta


def yaml_of(png: Path) -> Path:
    """NAME.yaml beside NAME.png. Not with_suffix: a ROM file name may hold a dot."""
    return png.with_name(png.name[:-len(".png")] + ".yaml")


def png_of(meta: Path) -> Path:
    return meta.with_name(meta.name[:-len(".yaml")] + ".png")


def pack_file(yaml_path: Path) -> bytes:
    """The bytes of the file a YAML file and the PNG beside it describe. A
    YAML file written by hand for a new file may leave out `size`, the
    tile bytes before compression: width x height x bpp / 8."""
    meta = yaml.safe_load(yaml_path.read_text())
    png = png_of(yaml_path)
    rows, _ = read_png(png)
    if (len(rows[0]), len(rows)) != (meta["width"], meta["height"]):
        raise ValueError(f"{png}: {len(rows[0])}x{len(rows)}, "
                         f"layout {meta['layout']} needs {meta['width']}x{meta['height']}")
    size = meta.get("size", meta["width"] * meta["height"] * meta["bpp"] // 8)
    pieces = layout_pieces(meta["layout"], size, meta["bpp"])
    raw = pixels_to_tiles(rows, pieces, meta["bpp"])
    return lz77_pack(raw) if meta["compression"] == "lz77" else raw


# ---------------------------------------------------------------------------
# ROM data: symbols, CO names, gCoGraphics


def arm9() -> bytes:
    path = ROOT / ARM9
    if not path.exists():
        sys.exit(f"error: {ARM9} not found; run make extract")
    return path.read_bytes()


def symbol(name: str) -> tuple[int, int]:
    """Address and size of a main symbol from symbols.txt."""
    for line in (ROOT / SYMBOLS).read_text().splitlines():
        if line.startswith(name + " "):
            addr = int(re.search(r"addr:(0x[0-9a-f]+)", line).group(1), 16)
            m = re.search(r"\[(\d+)\]", line)
            return addr, int(m.group(1)) if m else 0
    sys.exit(f"error: {name} is not in {SYMBOLS}")


def co_names() -> list[str]:
    """Names of enum CoId, CO_NONE first, up to CO_COUNT."""
    text = (ROOT / CO_HEADER).read_text()
    body = text[text.index("enum CoId {"):]
    names = []
    for m in re.finditer(r"^\s*CO_([A-Z_]+)\s*[,=]", body, re.M):
        if m.group(1) == "COUNT":
            break
        names.append(m.group(1))
    return names


# Words of one gCoGraphics row (include/game/co_graphics.h, the alias list
# in config/AWRE/aliases.txt): file names under /syogun, then pointers into
# main's data.
ROW_WORDS = 21
ROW_FULL_TOP = (0, 2)          # pictures[v].name, v = 0, 1
ROW_FULL_BOTTOM = (1, 3)       # pictures[v].unk_04
ROW_FACE = (4, 7)              # 3 moods from here, per variant (Co_LoadLargePicture)
ROW_BATTLE = (10, 13)          # 3 moods from here, per variant (Co_LoadMoodPicture)
ROW_SMALL = (16, 17)           # pointers to 0x180 bytes of tiles
ROW_MEDIUM = (18, 19)          # file names (Co_QueueArmyMediumPicture)
ROW_PALETTES = 20              # pointer to 8 palettes of 16 colours

# Map panel faces (CoPanel_Draw): 0x100 bytes per CO from CO 1, per variant.
HUD_FACES = ("gCoPanelFaces", "gCoPanelFacesAlt")


class Main:
    def __init__(self):
        self.data = arm9()

    def word(self, addr: int) -> int:
        return struct.unpack_from("<I", self.data, addr - ARM9_BASE)[0]

    def bytes(self, addr: int, size: int) -> bytes:
        return self.data[addr - ARM9_BASE:addr - ARM9_BASE + size]

    def string(self, addr: int) -> str:
        start = addr - ARM9_BASE
        return self.data[start:self.data.index(b"\0", start)].decode("ascii")


def cmd_co(args) -> int:
    names = co_names()
    co = int(args.co, 0) if args.co[0].isdigit() else names.index(args.co.upper().replace(" ", "_"))
    main = Main()
    table, table_size = symbol("gCoGraphics")
    row_count = table_size // (ROW_WORDS * 4)
    if not 0 <= co < row_count:
        sys.exit(f"error: CO id {co} is past the {row_count} rows of gCoGraphics")
    row = [main.word(table + (co * ROW_WORDS + k) * 4) for k in range(ROW_WORDS)]
    name = names[co] if co < len(names) else f"ROW{co}"
    out = ROOT / args.out / "co" / f"{co:02d}_{name}"
    palettes_at = row[ROW_PALETTES]
    palettes = [bgr555_to_rgb(main.bytes(palettes_at + 32 * k, 32)) for k in range(8)]
    pal = palettes[args.palette]
    pal_from = f"main:{palettes_at + 32 * args.palette:#010x}"
    written = []

    def from_file(word_index: int, png: str, layout: str, extra: dict) -> Path | None:
        ptr = row[word_index]
        if not ptr:
            return None
        file = f"syogun/{main.string(ptr)}"
        unpack_bytes((ROOT / FILES / file).read_bytes(), out / png, source=f"files/{file}", layout=layout,
                     palette=pal, palette_from=pal_from, extra=extra)
        written.append(out / png)
        return out / png

    def from_main(addr: int, size: int, png: str, layout: str, extra: dict):
        unpack_bytes(main.bytes(addr, size), out / png, source=f"main:{addr:#010x}", layout=layout,
                     palette=pal, palette_from=pal_from, extra=extra)
        written.append(out / png)

    for v in (0, 1):
        top = from_file(ROW_FULL_TOP[v], f"full_v{v}_top.png", "co_full_top", {"variant": v})
        bottom = from_file(ROW_FULL_BOTTOM[v], f"full_v{v}_bottom.png", "co_full_bottom", {"variant": v})
        if top and bottom:  # the whole figure as the tag cut-in shows it, for viewing only
            rows = read_png(top)[0] + read_png(bottom)[0]
            write_png(out / f"full_v{v}.png", rows, pal)
            written.append(out / f"full_v{v}.png")
        for mood in range(3):
            from_file(ROW_FACE[v] + mood, f"face_v{v}_m{mood}.png", "co_face", {"variant": v, "mood": mood})
            from_file(ROW_BATTLE[v] + mood, f"battle_v{v}_m{mood}.png", "co_battle", {"variant": v, "mood": mood})
        from_file(ROW_MEDIUM[v], f"medium_v{v}.png", "co_medium", {"variant": v})
        if row[ROW_SMALL[v]]:
            from_main(row[ROW_SMALL[v]], 0x180, f"small_v{v}.png", "co_small", {"variant": v})
        if 1 <= co < len(names):
            from_main(symbol(HUD_FACES[v])[0] + (co - 1) * 0x100, 0x100, f"hud_v{v}.png", "co_hud",
                      {"variant": v})

    # Palette sheet: one row of 16 swatches (8x8) per palette.
    sheet = [bytearray(k * 16 + x // 8 for x in range(128)) for k in range(8) for _ in range(8)]
    write_png(out / "palettes.png", sheet, [c for p in palettes for c in p], transparent0=False)
    written.append(out / "palettes.png")
    for path in written:
        print(path.relative_to(ROOT) if path.is_relative_to(ROOT) else path)
    return 0


# ---------------------------------------------------------------------------
# Commands


def parse_palette(spec: str | None) -> tuple[list | None, str | None]:
    """main:ADDR[:COUNT], or FILE[:OFFSET] holding BGR555 colours; 16 colours unless COUNT says more."""
    if not spec:
        return None, None
    parts = spec.split(":")
    if parts[0] == "main":
        addr = int(parts[1], 16)
        count = int(parts[2]) if len(parts) > 2 else 16
        return bgr555_to_rgb(Main().bytes(addr, count * 2)), spec
    path = ROOT / parts[0] if not Path(parts[0]).is_absolute() else Path(parts[0])
    offset = int(parts[1], 0) if len(parts) > 1 else 0
    return bgr555_to_rgb(path.read_bytes()[offset:offset + 32]), spec


def cmd_unpack(args) -> int:
    if args.file is None:
        return unpack_rom(ROOT / args.out if args.out else ROOT / PICTURES)
    src = Path(args.file)
    data = src.read_bytes()
    palette, palette_from = parse_palette(args.palette)
    try:
        source = src.resolve().relative_to((ROOT / "extract").resolve()).as_posix()
    except ValueError:
        source = str(src)
    layout = args.layout
    if args.out:
        out = Path(args.out) / (src.name + ".png")
    elif source.startswith("files/"):  # under its ROM path, where the build takes it from
        path = source[len("files/"):]
        out = ROOT / PICTURES / (path + ".png")
        layout = layout or picture_layout(path, len(describe(data)[0]))
        if palette is None:  # as `unpack` with no file writes it
            main = Main()
            addr = co_palettes(main).get(path)
            if addr:
                palette, palette_from = bgr555_to_rgb(main.bytes(addr, 32)), f"main:{addr:#010x}"
    else:  # not into PICTURES: the build would add it to the ROM
        out = ROOT / EXTRACTED / (src.name + ".png")
    meta = unpack_bytes(data, out, source=source, layout=layout, bpp=args.bpp, palette=palette,
                        palette_from=palette_from)
    print(f"{out}: {meta['width']}x{meta['height']}, {meta['layout']}, {meta['compression']}")
    return 0


# ---------------------------------------------------------------------------
# Every picture file of the ROM, and the build


def rom_pictures() -> list[str]:
    """Paths under extract/files/ of the files `unpack` writes with no file
    named: LZ77 streams of whole 4-bit tiles. Left out: the res_* containers
    (several parts in one stream) and files that are not compressed (palettes
    and other data next to the tiles)."""
    out = []
    for f in sorted((ROOT / FILES).rglob("*")):
        if not f.is_file() or f.name.startswith("res_"):
            continue
        raw, compression = describe(f.read_bytes())
        if compression == "lz77" and len(raw) % 32 == 0:
            out.append(f.relative_to(ROOT / FILES).as_posix())
    return out


def picture_layout(path: str, size: int) -> str:
    """The CO layouts for /syogun, where the sizes say which picture a file
    holds; a grid elsewhere, where a size can mean anything."""
    if path.startswith("syogun/"):
        return default_layout(size)
    tiles = size // 32
    return f"grid:{next(w for w in (16, 8, 4, 2, 1) if tiles % w == 0)}"


def co_palettes(main: "Main") -> dict[str, int]:
    """Address of palette 0 of the gCoGraphics row that names each /syogun
    file, so the PNG shows the CO's colours. Only the colour indices go back
    into the file."""
    table, table_size = symbol("gCoGraphics")
    out = {}
    for co in range(table_size // (ROW_WORDS * 4)):
        row = [main.word(table + (co * ROW_WORDS + k) * 4) for k in range(ROW_WORDS)]
        words = [*ROW_FULL_TOP, *ROW_FULL_BOTTOM, *ROW_MEDIUM,
                 *(w + m for w in ROW_FACE + ROW_BATTLE for m in range(3))]
        for k in words:
            if row[k]:
                out.setdefault(f"syogun/{main.string(row[k])}", row[ROW_PALETTES])
    return out


def unpack_rom(out: Path) -> int:
    main = Main()
    palettes = co_palettes(main)
    paths = rom_pictures()
    for path in paths:
        data = (ROOT / FILES / path).read_bytes()
        palette, palette_from = None, None
        if path in palettes:
            palette, palette_from = bgr555_to_rgb(main.bytes(palettes[path], 32)), f"main:{palettes[path]:#010x}"
        unpack_bytes(data, out / (path + ".png"), source=f"files/{path}",
                     layout=picture_layout(path, len(describe(data)[0])), palette=palette, palette_from=palette_from)
    (out / UNPACKED).write_text("")
    shown = out.relative_to(ROOT) if out.is_relative_to(ROOT) else out
    print(f"unpack: {len(paths)} picture files in {shown}")
    return 0


def build_pictures(directory: Path = PICTURES) -> dict[str, str]:
    """The files the build packs, by ROM path under files/: every YAML file
    under extracted/AWRE/assets/gfx/files/ with a ROM path from its place.
    The PNG beside each must exist; configure.py lists both as inputs."""
    root = ROOT / directory
    return {p.relative_to(root).as_posix()[:-len(".yaml")]:
            (p.relative_to(ROOT) if p.is_relative_to(ROOT) else p).as_posix()
            for p in sorted(root.rglob("*.yaml"))}


def cmd_pack(args) -> int:
    yaml_path = Path(args.yaml)
    data = pack_file(yaml_path)
    out = Path(args.out) if args.out else yaml_path.with_name(yaml_path.name[:-len(".yaml")] + ".bin")
    write_if_changed(out, data)
    meta = yaml.safe_load(yaml_path.read_text())
    same = hashlib.sha1(data).hexdigest() == meta.get("sha1")
    if not args.quiet:
        print(f"{out}: {len(data)} bytes{', same as the ROM' if same else ', changed'}")
    if args.palette_out:
        _, palette = read_png(png_of(yaml_path))
        colours = (palette + [(0, 0, 0)] * 16)[:1 << meta["bpp"]]
        Path(args.palette_out).write_bytes(rgb_to_bgr555(colours))
        print(f"{args.palette_out}: {len(colours)} colours, BGR555")
    return 0


def cmd_check(args) -> int:
    directory = ROOT / args.dir
    files = sorted(p for p in directory.iterdir() if p.is_file())
    if not files:
        sys.exit(f"error: no files in {args.dir}; run make extract")
    counts = {"lz77": 0, "none": 0, "skipped": 0}
    bad = []
    with tempfile.TemporaryDirectory() as tmp:
        for f in files:
            data = f.read_bytes()
            raw, compression = describe(data)
            if len(raw) % 32 or (compression == "none" and not args.raw):
                counts["skipped"] += 1  # not tile data, or not compressed (raw files only with --raw)
                continue
            png = Path(tmp) / (f.name + ".png")
            unpack_bytes(data, png, source=str(f))
            if pack_file(yaml_of(png)) != data:
                bad.append(f.name)
            counts[compression] += 1
    print(f"{args.dir}: {counts['lz77']} LZ77 files and {counts['none']} raw files come back the same, "
          f"{counts['skipped']} skipped, {len(bad)} differ")
    for name in bad:
        print(f"  differs: {name}")
    return 1 if bad else 0


def cmd_layouts(args) -> int:
    for name, pieces in LAYOUTS.items():
        width = max(x + w for x, _, w, _ in pieces)
        height = max(y + h for _, y, _, h in pieces)
        tiles = sum(w * h for _, _, w, h in pieces) // 64
        size = next((hex(s) for s, n in LAYOUT_BY_SIZE.items() if n == name), "-")
        print(f"{name:16} {width}x{height}  {tiles} tiles  {len(pieces)} pieces  default for {size} bytes")
    print("grid:N           N tiles wide, rows filled left to right")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(required=True)
    p = sub.add_parser("unpack", help="one tile file, or every picture file of the ROM, to indexed PNG and YAML files")
    p.add_argument("file", nargs="?", help="default: every picture file of the ROM, under its ROM path")
    p.add_argument("--out", help=f"default: {PICTURES} under the ROM path for a file of extract/files/, else {EXTRACTED}")
    p.add_argument("--layout", help="a name from `gfx.py layouts` or grid:N (default: by size)")
    p.add_argument("--bpp", type=int, choices=(4, 8), default=4)
    p.add_argument("--palette", help="main:ADDR[:COUNT] or FILE[:OFFSET] (default: greys)")
    p.set_defaults(func=cmd_unpack)
    p = sub.add_parser("pack", help="the PNG beside a YAML file back to the file's bytes")
    p.add_argument("yaml")
    p.add_argument("--out", help="default: the YAML path with .bin")
    p.add_argument("--palette-out", help="also write the PNG's palette as BGR555 (32 bytes for 4 bits)")
    p.add_argument("--quiet", action="store_true", help="print nothing (the build's GFX step)")
    p.set_defaults(func=cmd_pack)
    p = sub.add_parser("check", help="unpack and pack every tile file of a directory; compare bytes")
    p.add_argument("--dir", default=str(FILES / "syogun"))
    p.add_argument("--raw", action="store_true", help="also files that are not compressed")
    p.set_defaults(func=cmd_check)
    p = sub.add_parser("co", help="every picture of one CO")
    p.add_argument("co", help="CO id or name from enum CoId (VON_BOLT, flak, 26)")
    p.add_argument("--out", default=str(EXTRACTED))
    p.add_argument("--palette", type=int, default=0, choices=range(8), help="palette variant (default 0)")
    p.set_defaults(func=cmd_co)
    p = sub.add_parser("layouts", help="list the layouts")
    p.set_defaults(func=cmd_layouts)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
