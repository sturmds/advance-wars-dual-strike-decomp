#!/usr/bin/env python3
"""Check tools/gfx.py.

    python3 tools/tests/test_gfx.py

- LZ77: made-up data (runs, repeats at distance 1 and 2, a long random part)
  comes back from lz77_pack and lz77_unpack unchanged, the output never uses
  distance 1, and its length is a multiple of 4.
- Tiles: a picture written with each layout through tiles_to_pixels and
  pixels_to_tiles gives the same bytes; a colour index above 15 stops a
  4-bit pack.
- PNG: write_png and read_png give back the rows and the palette, and
  read_png reads a 4-bit PNG with every filter type.
- Files: write_if_changed replaces a symbolic link instead of writing
  through it (build/rom/files/ links to extract/files/); a YAML file
  without `size` packs as one with it; a ROM name with a dot keeps it in
  its PNG and YAML names; build_pictures maps each YAML file to its ROM
  path; picture_layout uses the CO layouts only for /syogun.
- With extract/ present: every file `unpack` writes for the build (every
  picture file of the ROM, all of /syogun's LZ77 files among them) comes
  back from its PNG and YAML file as the same bytes. Skipped with a message
  when there is no extract/.
"""

import concurrent.futures
import multiprocessing
import random
import struct
import sys
import tempfile
import zlib
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))
import gfx  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent.parent
failures = []


def expect(what: str, got, want) -> None:
    print(f"{what}: {'ok' if got == want else 'FAIL'}")
    if got != want:
        print(f"  got:  {got!r}\n  want: {want!r}")
        failures.append(what)


def distances(packed: bytes) -> set[int]:
    """Distances of the references in an LZ77 stream."""
    size = int.from_bytes(packed[1:4], "little")
    seen, i, out = set(), 4, 0
    while out < size:
        flags = packed[i]
        i += 1
        for bit in range(8):
            if out >= size:
                break
            if flags & (0x80 >> bit):
                ref = packed[i] << 8 | packed[i + 1]
                seen.add((ref & 0xFFF) + 1)
                out += (ref >> 12) + 3
                i += 2
            else:
                out += 1
                i += 1
    return seen


rng = random.Random(1)
data = bytes(40) + b"ab" * 30 + bytes(rng.randrange(4) for _ in range(5000)) + b"\x11" * 300
packed = gfx.lz77_pack(data)
expect("lz77 round trip", gfx.lz77_unpack(packed)[0], data)
expect("lz77 never uses distance 1", 1 in distances(packed), False)
expect("lz77 padded to 4", len(packed) % 4, 0)
expect("lz77 stream length", gfx.lz77_unpack(packed)[1], len(packed))

for name in gfx.LAYOUTS:
    pieces = gfx.LAYOUTS[name]
    size = sum(w * h for _, _, w, h in pieces) // 2
    raw = bytes(rng.randrange(256) for _ in range(size))
    _, _, rows = gfx.tiles_to_pixels(raw, gfx.layout_pieces(name, size, 4), 4)
    expect(f"layout {name} round trip", gfx.pixels_to_tiles(rows, pieces, 4), raw)
rows = [bytearray(8) for _ in range(8)]
rows[3][5] = 16
try:
    gfx.pixels_to_tiles(rows, gfx.grid(1, 1), 4)
    expect("index 16 stops a 4-bit pack", "packed", "error")
except ValueError:
    expect("index 16 stops a 4-bit pack", "error", "error")

with tempfile.TemporaryDirectory() as tmp:
    png = Path(tmp) / "a.png"
    rows = [bytearray((x + y) % 16 for x in range(24)) for y in range(16)]
    palette = gfx.grey_palette(16)
    gfx.write_png(png, rows, palette)
    expect("png round trip", gfx.read_png(png), (rows, palette))

    # A 4-bit PNG with one row per filter type, as an image editor may save it.
    width = 6
    plain = [bytes(rng.randrange(256) for _ in range(width // 2)) for _ in range(5)]
    raw, prev = b"", bytes(width // 2)
    for kind, line in enumerate(plain):
        enc = bytearray()
        for k, v in enumerate(line):
            a = line[k - 1] if k else 0
            b, c = prev[k], prev[k - 1] if k else 0
            p = a + b - c
            pred = [0, a, b, (a + b) // 2,
                    a if abs(p - a) <= abs(p - b) and abs(p - a) <= abs(p - c) else b if abs(p - b) <= abs(p - c) else c][kind]
            enc.append((v - pred) & 0xFF)
        raw += bytes([kind]) + bytes(enc)
        prev = line
    body = b"\x89PNG\r\n\x1a\n"
    body += gfx.png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, 5, 4, 3, 0, 0, 0))
    body += gfx.png_chunk(b"PLTE", bytes(48))
    body += gfx.png_chunk(b"IDAT", zlib.compress(raw))
    body += gfx.png_chunk(b"IEND", b"")
    png.write_bytes(body)
    want = [bytearray(v for b in line for v in (b >> 4, b & 0xF)) for line in plain]
    expect("png 4-bit, filters 0-4", gfx.read_png(png)[0], want)

with tempfile.TemporaryDirectory() as tmp:
    tmp = Path(tmp)
    target = tmp / "original"
    target.write_bytes(b"rom")
    link = tmp / "built"
    link.symlink_to(target)
    gfx.write_if_changed(link, b"rom")
    expect("write_if_changed replaces a link, even with the same bytes",
           (link.is_symlink(), link.read_bytes(), target.read_bytes()), (False, b"rom", b"rom"))
    expect("write_if_changed leaves an unchanged file", gfx.write_if_changed(link, b"rom"), False)

    rows = [bytearray((x * 3 + y) % 16 for x in range(16)) for y in range(16)]
    gfx.write_png(tmp / "pics/syogun/a.b.png", rows, gfx.grey_palette(16))
    meta = {"compression": "lz77", "bpp": 4, "layout": "grid:2", "width": 16, "height": 16}
    (tmp / "pics/syogun/a.b.yaml").write_text(gfx.yaml.safe_dump(meta))
    want = gfx.lz77_pack(gfx.pixels_to_tiles(rows, gfx.grid(2, 4), 4))
    expect("pack without size", gfx.pack_file(tmp / "pics/syogun/a.b.yaml"), want)
    expect("png and yaml names keep a dot", (gfx.yaml_of(Path("x/a.b.png")), gfx.png_of(Path("x/a.b.yaml"))),
           (Path("x/a.b.yaml"), Path("x/a.b.png")))
    (tmp / "pics/sys").mkdir()
    (tmp / "pics/sys/c.yaml").write_text("")
    expect("build_pictures", gfx.build_pictures(tmp / "pics"),
           {"syogun/a.b": (tmp / "pics/syogun/a.b.yaml").as_posix(), "sys/c": (tmp / "pics/sys/c.yaml").as_posix()})
expect("picture_layout", [gfx.picture_layout("syogun/076", 0x480), gfx.picture_layout("battle/000", 0x480)],
       ["co_face", "grid:4"])

def round_trip(path: str, tmp: str) -> bool:
    """A ROM file through the PNG and YAML files `unpack` writes, and back."""
    data = (ROOT / gfx.FILES / path).read_bytes()
    png = Path(tmp) / (path + ".png")
    gfx.unpack_bytes(data, png, source=path, layout=gfx.picture_layout(path, len(gfx.describe(data)[0])))
    return gfx.pack_file(gfx.yaml_of(png)) == data


if (ROOT / gfx.FILES / "syogun").is_dir():
    paths = gfx.rom_pictures()
    syogun = [p for p in paths if p.startswith("syogun/")]
    with tempfile.TemporaryDirectory() as tmp, concurrent.futures.ProcessPoolExecutor(
            mp_context=multiprocessing.get_context("fork")) as pool:  # fork: the workers do not run this file again
        same = pool.map(round_trip, paths, [tmp] * len(paths), chunksize=16)
        bad = [path for path, ok in zip(paths, same) if not ok]
    expect("every /syogun LZ77 file is a picture file", len(syogun), 508)
    expect(f"{len(paths)} picture files come back the same", bad, [])
else:
    print("picture files of the ROM: skipped, no extract/ (run make extract)")

if failures:
    sys.exit(f"test_gfx: {len(failures)} failed")
