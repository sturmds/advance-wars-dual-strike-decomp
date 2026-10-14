#!/usr/bin/env python3
"""Sound archive: unpack files/data/sound_data.sdat into extracted/, pack it back.

    sound.py unpack [--sdat SDAT] [--out extracted/AWRE/assets/sound]   archive -> one file per FAT entry + sound.yaml
    sound.py pack [--in extracted/AWRE/assets/sound] [--out build/sound/sound_data.sdat]
    sound.py check [--sdat SDAT]                 unpack to a temporary directory, pack, compare the SHA-1
    sound.py list [--sdat SDAT] [--kind seq] [--grep NAME]   id, name, bank, wave archives and size of each entry
    sound.py export [--out extracted/AWRE/music] [--format mp3] [--force] [--auto]
                                                 every song, sound effect and stream as an audio file

The archive is a NitroSystem SDAT (docs/sound.md). Everything in it is game
data from the builder's ROM: the files, the names and the manifest stay in
extracted/ or build/ and never enter git (tenet 9). The ROM build still
copies sound_data.sdat from extract/ (issue 0050); `pack` is not part of it.

  extracted/AWRE/assets/sound/
      sound.yaml                 every INFO entry with its SYMB name, fields and file
      seq/NNN_NAME.sseq          sequences (scores)
      seqarc/NN_NAME.ssar        sequence archives (sound effects)
      bank/NNN_NAME.sbnk         instrument banks
      wavearc/NNN_NAME.swar      wave archives (samples)
      strm/NN_NAME.strm          streams

Each FAT entry is written once, named after the first INFO entry that uses it
(INFO order: seq, seqarc, bank, wavearc, strm); the other entries name the
same file in sound.yaml. `pack` rebuilds the layout the ROM has: SYMB records
then strings in record order, INFO records each followed by its entries,
files in FAT order on 32-byte boundaries (docs/sound.md).

`export` renders audio with zxtune123 (tools.lock), which plays the archive
through an emulation of the DS sound engine, and encodes it with ffmpeg (a
system package). See docs/sound.md, "Export".

  extracted/AWRE/music/
      bgm/NN Title.mp3           songs, titled as in the Sound Room; the stream too
      se/NNN_NAME.mp3            sound effects that are sequences
      se/NN_ARCHIVE/NNN_NAME.mp3 sound effects inside sequence archives
"""

import argparse
import concurrent.futures
import hashlib
import os
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
SDAT = Path("extract/files/data/sound_data.sdat")
EXTRACTED = Path("extracted/AWRE/assets/sound")  # gitignored
PACKED = Path("build/sound/sound_data.sdat")
MANIFEST = "sound.yaml"
MUSIC = Path("extracted/AWRE/music")  # gitignored
NCSF_DIR = Path("build/sound/ncsf")
ZXTUNE = Path(".cache/tools/zxtune/bin/zxtune123")

# Record order in SYMB and INFO.
KINDS = ("seq", "seqarc", "bank", "wavearc", "player", "group", "player2", "strm")
# Kinds whose entries point at a FAT entry: extension and digits of the id in file names.
FILE_KINDS = {"seq": ("sseq", 3), "seqarc": ("ssar", 2), "bank": ("sbnk", 3), "wavearc": ("swar", 3), "strm": ("strm", 2)}
HEADER_SIZE = 0x40
BLOCK_HEADER = 0x40                # magic, size, 8 record offsets, 24 reserved bytes
FILE_ALIGN = 32
NO_WAVEARC = 0xffff
# INFO entry of each kind: struct format and field names. The names are the
# C names in include/game/sound_arc.h where it has the struct (SoundArcSeqInfo,
# SoundArcBankInfo, SoundArcStrmInfo, SoundArcPlayerGroupInfo). A wave
# archive's word holds the file id in its low 24 bits and `flags` on top.
# Fields in OPTIONAL are 0 in our ROM; sound.yaml leaves them out when 0.
ENTRY = {
    "seq": ("<IHBBBBH", ("fileId", "bankNo", "volume", "channelPrio", "playerPrio", "playerNo", "pad_0a")),
    "seqarc": ("<I", ("fileId",)),
    "bank": ("<IHHHH", ("fileId", "w0", "w1", "w2", "w3")),
    "wavearc": ("<I", ("fileId",)),
    "player": ("<BBHI", ("maxPlayers", "pad_01", "trackChannelMask", "heapSize")),
    "player2": ("<B16s7s", ("count", "channels", "pad_11")),
    "strm": ("<IBBBB4s", ("fileId", "volume", "priority", "slotNo", "flags", "pad_08")),
}
OPTIONAL = ("flags", "pad_01", "pad_08", "pad_0a", "pad_11")


def u32(data: bytes, offset: int) -> int:
    return struct.unpack_from("<I", data, offset)[0]


def align(n: int, a: int) -> int:
    return (n + a - 1) // a * a


# ---------------------------------------------------------------- reading the archive

def cstring(data: bytes, offset: int) -> str:
    return data[offset:data.index(b"\0", offset)].decode("ascii")


def read_entry(kind: str, raw: bytes) -> dict:
    """INFO entry fields as sound.yaml writes them."""
    fmt, names = ENTRY[kind]
    e = dict(zip(names, struct.unpack(fmt, raw)))
    if kind == "wavearc":
        e["flags"] = e["fileId"] >> 24
        e["fileId"] &= 0xffffff
    if kind == "bank":
        # 0xffff marks an unused slot: null, and left out at the end.
        arcs = [None if e[f"w{i}"] == NO_WAVEARC else e[f"w{i}"] for i in range(4)]
        for i in range(4):
            del e[f"w{i}"]
        while arcs and arcs[-1] is None:
            arcs.pop()
        e["waveArc"] = arcs
    if kind == "player2":
        # 16 channel bytes, the first `count` used, the rest 0xff.
        count, channels = e.pop("count"), e["channels"]
        e["channels"] = list(channels[:count])
        if channels[count:] != b"\xff" * (16 - count):
            e["channelsRest"] = channels[count:].hex()
    for key in OPTIONAL:
        v = e.get(key)
        if isinstance(v, bytes):
            v = e[key] = v.hex() if v.strip(b"\0") else 0
        if v == 0:
            e.pop(key, None)
    return e


def write_entry(kind: str, e: dict, where: str) -> bytes:
    fmt, names = ENTRY[kind]
    v = dict(e)
    if kind == "wavearc":
        v["fileId"] = v.get("fileId", 0) | v.pop("flags", 0) << 24
    if kind == "bank":
        arcs = [NO_WAVEARC if w is None else w for w in v.pop("waveArc", [])]
        if len(arcs) > 4:
            sys.exit(f"error: {where}: a bank has at most 4 wave archives")
        arcs += [NO_WAVEARC] * (4 - len(arcs))
        v.update({f"w{i}": w for i, w in enumerate(arcs)})
    if kind == "player2":
        channels = bytes(v.get("channels", []))
        v["count"] = len(channels)
        v["channels"] = channels + bytes.fromhex(v.pop("channelsRest", "ff" * (16 - len(channels))))
    unknown = set(v) - set(names)
    if unknown:
        sys.exit(f"error: {where}: no field {sorted(unknown)[0]} in a {kind} entry")
    out = []
    for name, code in zip(names, re.findall(r"\d*[a-zA-Z]", fmt[1:])):
        x = v.get(name, 0)
        if code.endswith("s") and not isinstance(x, bytes):
            x = bytes.fromhex(x) if isinstance(x, str) else bytes(int(code[:-1]))
        out.append(x)
    try:
        return struct.pack(fmt, *out)
    except struct.error as err:
        sys.exit(f"error: {where}: {err}")


class Archive:
    """The parts of an SDAT that sound.yaml and the files hold."""

    def __init__(self, data: bytes) -> None:
        if data[:4] != b"SDAT" or struct.unpack_from("<HHIHH", data, 4) != (0xfeff, 0x100, len(data), HEADER_SIZE, 4):
            sys.exit("error: not an SDAT v1.0 archive with 4 blocks")
        self.data = data
        symb, _, info, _, fat, _, file_, _ = struct.unpack_from("<8I", data, 0x10)
        if not symb:
            sys.exit("error: the archive has no SYMB block (names stripped); not handled")
        for off, magic in ((symb, b"SYMB"), (info, b"INFO"), (fat, b"FAT "), (file_, b"FILE")):
            if data[off:off + 4] != magic:
                sys.exit(f"error: no {magic.decode()} block at {off:#x}")
        # Names: kind -> list of names (None: no name); seqarc also has sub-names.
        self.names: dict[str, list] = {}
        self.subnames: list = []
        for k, kind in enumerate(KINDS):
            rec = symb + u32(data, symb + 8 + 4 * k)
            n = u32(data, rec)
            names = []
            for i in range(n):
                if kind == "seqarc":
                    name_off, sub_off = struct.unpack_from("<II", data, rec + 4 + 8 * i)
                    subs = None
                    if sub_off:
                        sub = symb + sub_off
                        subs = [cstring(data, symb + o) if o else None
                                for o in struct.unpack_from(f"<{u32(data, sub)}I", data, sub + 4)]
                    self.subnames.append(subs)
                else:
                    name_off = u32(data, rec + 4 + 4 * i)
                names.append(cstring(data, symb + name_off) if name_off else None)
            self.names[kind] = names
        # INFO entries: kind -> list of dicts (None: missing entry).
        self.entries: dict[str, list] = {}
        for k, kind in enumerate(KINDS):
            rec = info + u32(data, info + 8 + 4 * k)
            offsets = struct.unpack_from(f"<{u32(data, rec)}I", data, rec + 4)
            if kind == "group":
                if offsets:
                    sys.exit("error: the archive has GROUP entries; not handled (ours has none)")
                self.entries[kind] = []
                continue
            size = struct.calcsize(ENTRY[kind][0])
            self.entries[kind] = [read_entry(kind, data[info + o:info + o + size]) if o else None for o in offsets]
        # FAT: file id -> bytes.
        count = u32(data, fat + 8)
        self.files = []
        for i in range(count):
            off, size = struct.unpack_from("<II", data, fat + 12 + 16 * i)
            self.files.append(data[off:off + size])

    def name(self, kind: str, i: int) -> str:
        names = self.names[kind]
        return names[i] if i < len(names) and names[i] else f"{kind.upper()}_{i:03d}"


# ---------------------------------------------------------------- writing the archive

def build(names: dict[str, list], subnames: list, entries: dict[str, list], files: list[bytes], version: int = 0x100) -> bytes:
    """An SDAT laid out the way the ROM's is (docs/sound.md, "Layout")."""
    # SYMB: the 8 records, the seqarc sub-records right after the seqarc
    # record, then the strings in the order the records name them (a
    # sequence archive's name before its sub-names).
    rec_offsets, records, strings = [], bytearray(), bytearray()
    refs: list[tuple[int, str | None]] = []   # (offset of a name field in records, name), in string order

    def table(items: list[str | None]) -> list[tuple[int, str | None]]:
        records.extend(struct.pack("<I", len(items)))
        at = len(records)
        records.extend(bytes(4 * len(items)))
        return [(at + 4 * i, name) for i, name in enumerate(items)]

    for kind in KINDS:
        rec_offsets.append(BLOCK_HEADER + len(records))
        if kind != "seqarc":
            refs += table(names.get(kind, []))
            continue
        pairs = len(records) + 4
        records.extend(struct.pack("<I", len(names[kind])) + bytes(8 * len(names[kind])))
        for i, name in enumerate(names[kind]):
            refs.append((pairs + 8 * i, name))
            if subnames[i] is not None:
                struct.pack_into("<I", records, pairs + 8 * i + 4, BLOCK_HEADER + len(records))
                refs += table(subnames[i])
    for at, name in refs:
        if name is not None:
            struct.pack_into("<I", records, at, BLOCK_HEADER + len(records) + len(strings))
            strings += name.encode("ascii") + b"\0"
    symb_size = BLOCK_HEADER + len(records) + len(strings)
    symb = bytearray(b"SYMB" + struct.pack("<I", align(symb_size, 4)) + struct.pack("<8I", *rec_offsets) + bytes(24))
    symb += records + strings
    symb += bytes(align(symb_size, 4) - symb_size)

    # INFO: each record followed by its entries.
    info_body = bytearray()
    rec_offsets = []
    for kind in KINDS:
        rec_offsets.append(BLOCK_HEADER + len(info_body))
        items = entries.get(kind, [])
        at = BLOCK_HEADER + len(info_body) + 4 + 4 * len(items)
        offsets, raw = [], bytearray()
        for i, e in enumerate(items):
            if e is None:
                offsets.append(0)
                continue
            offsets.append(at + len(raw))
            raw += write_entry(kind, e, f"{kind} {i}")
        info_body += struct.pack(f"<I{len(items)}I", len(items), *offsets) + raw
    info = b"INFO" + struct.pack("<I", BLOCK_HEADER + len(info_body)) + struct.pack("<8I", *rec_offsets) + bytes(24)
    info += info_body

    # FAT and FILE: files in id order, each on a 32-byte boundary of the archive.
    symb_at = HEADER_SIZE
    info_at = symb_at + len(symb)
    fat_at = info_at + len(info)
    fat_size = 12 + 16 * len(files)
    file_at = fat_at + fat_size
    body = bytearray(b"FILE" + bytes(4) + struct.pack("<II", len(files), 0))
    fat = bytearray(b"FAT " + struct.pack("<II", fat_size, len(files)))
    for f in files:
        pos = align(file_at + len(body), FILE_ALIGN)
        body += bytes(pos - file_at - len(body))
        fat += struct.pack("<IIII", pos, len(f), 0, 0)
        body += f
    body += bytes(align(file_at + len(body), FILE_ALIGN) - file_at - len(body))
    struct.pack_into("<I", body, 4, len(body))
    total = file_at + len(body)
    # The header gives SYMB's size without its padding; the block's own size field includes it.
    header = b"SDAT" + struct.pack("<HHIHH", 0xfeff, version, total, HEADER_SIZE, 4)
    header += struct.pack("<8I", symb_at, symb_size, info_at, len(info), fat_at, fat_size, file_at, len(body)) + bytes(16)
    return bytes(header + symb + info + fat + body)


# ---------------------------------------------------------------- sound.yaml

def scalar(v) -> str:
    if v is None:
        return "null"
    if isinstance(v, str) and (not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_./]*", v) or yaml.safe_load(v) != v):
        return repr(v)                 # quoted: not a plain word, or YAML reads it as another type (NO, null)
    return str(v)


def flow(v) -> str:
    if isinstance(v, list):
        return "[" + ", ".join(flow(x) for x in v) + "]"
    if isinstance(v, dict):
        return "{" + ", ".join(f"{k}: {flow(x)}" for k, x in v.items()) + "}"
    return scalar(v)


def file_name(kind: str, i: int, name: str) -> str:
    ext, digits = FILE_KINDS[kind]
    return f"{kind}/{i:0{digits}d}_{re.sub(r'[^A-Za-z0-9_]', '_', name)}.{ext}"


def cmd_unpack(args) -> int:
    src = ROOT / args.sdat
    if not src.exists():
        sys.exit(f"error: {args.sdat} not found; run make extract")
    data = src.read_bytes()
    arc = Archive(data)
    out = ROOT / args.out
    # Only what unpack writes is removed, so a file under an old name does not stay behind.
    for kind in FILE_KINDS:
        shutil.rmtree(out / kind, ignore_errors=True)
    shutil.rmtree(out / "other", ignore_errors=True)
    out.mkdir(parents=True, exist_ok=True)
    paths: dict[int, str] = {}                 # file id -> file written
    lines = [f"# {args.sdat}: every INFO entry, in id order, with its SYMB name (docs/sound.md).",
             f"# sha1 {hashlib.sha1(data).hexdigest()}, {len(data)} bytes, {len(arc.files)} files",
             f"version: {data[6] | data[7] << 8:#x}"]
    for kind in KINDS:
        items = arc.entries[kind]
        lines.append(f"{kind}:" + ("" if items else " []"))
        for i, e in enumerate(items):
            name = arc.names[kind][i] if i < len(arc.names[kind]) else None
            row = {"id": i, "name": name}
            subs = {"sequences": arc.subnames[i] if i < len(arc.subnames) else None} if kind == "seqarc" else {}
            if e is None:
                lines.append(f"- {flow(row | subs)}")
                continue
            if kind in FILE_KINDS:
                fid = e["fileId"]
                if fid not in paths:
                    paths[fid] = file_name(kind, i, arc.name(kind, i))
                    path = out / paths[fid]
                    path.parent.mkdir(parents=True, exist_ok=True)
                    path.write_bytes(arc.files[fid])
                row["file"] = paths[fid]
            lines.append(f"- {flow(row | e | subs)}")
    # Names without an INFO entry (none in our ROM) and files no entry uses.
    extra = {k: v[len(arc.entries[k]):] for k, v in arc.names.items() if len(v) > len(arc.entries[k])}
    if extra:
        lines.append(f"extraNames: {flow(extra)}")
    unused = [i for i in range(len(arc.files)) if i not in paths]
    for fid in unused:
        paths[fid] = f"other/{fid:03d}.bin"
        (out / "other").mkdir(exist_ok=True)
        (out / paths[fid]).write_bytes(arc.files[fid])
    lines.append("unusedFiles:" + (" []" if not unused else ""))
    lines += [f"- {{fileId: {fid}, file: {paths[fid]}}}" for fid in unused]
    (out / MANIFEST).write_text("\n".join(lines) + "\n")
    counts = ", ".join(f"{len(arc.entries[k])} {k}" for k in KINDS if arc.entries[k])
    print(f"unpack: {counts}; {len(arc.files)} files in {args.out}")
    return 0


def load(src: Path) -> bytes:
    """The SDAT that sound.yaml and the files under src describe."""
    path = src / MANIFEST
    if not path.exists():
        sys.exit(f"error: {path} not found; run tools/sound.py unpack")
    doc = yaml.safe_load(path.read_text())
    names: dict[str, list] = {}
    entries: dict[str, list] = {}
    subnames: list = []
    files: dict[int, str] = {}

    def use(fid: int, rel: str, where: str) -> None:
        if files.setdefault(fid, rel) != rel:
            sys.exit(f"error: {where}: file id {fid} is {files[fid]} elsewhere, not {rel}")

    for kind in KINDS:
        rows = doc.get(kind) or []
        names[kind], entries[kind] = [], []
        for i, row in enumerate(rows):
            where = f"{MANIFEST}: {kind} {i}"
            row = dict(row)
            if row.pop("id", i) != i:
                sys.exit(f"error: {where}: ids must count up from 0 in order")
            names[kind].append(row.pop("name", None))
            if kind == "seqarc":
                subnames.append(row.pop("sequences", None))
            rel = row.pop("file", None)
            if not row:
                entries[kind].append(None)
                continue
            if kind in FILE_KINDS:
                if rel is None:
                    sys.exit(f"error: {where}: no file")
                use(row["fileId"], rel, where)
            entries[kind].append(row)
    for kind, extra in (doc.get("extraNames") or {}).items():
        names[kind] += extra
    for row in doc.get("unusedFiles") or []:
        use(row["fileId"], row["file"], f"{MANIFEST}: unusedFiles")
    if not files:
        sys.exit(f"error: {MANIFEST}: no files")
    if sorted(files) != list(range(len(files))):
        missing = sorted(set(range(max(files) + 1)) - set(files))
        sys.exit(f"error: {MANIFEST}: no entry uses file id {missing[0]}")
    data = []
    for fid in range(len(files)):
        path = src / files[fid]
        if not path.exists():
            sys.exit(f"error: {path} not found")
        data.append(path.read_bytes())
    return build(names, subnames, entries, data, doc.get("version", 0x100))


def cmd_pack(args) -> int:
    data = load(ROOT / args.input)
    out = ROOT / args.out
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_bytes(data)
    print(f"pack: {args.out}, {len(data)} bytes, sha1 {hashlib.sha1(data).hexdigest()}")
    return 0


def cmd_check(args) -> int:
    src = ROOT / args.sdat
    if not src.exists():
        sys.exit(f"error: {args.sdat} not found; run make extract")
    want = hashlib.sha1(src.read_bytes()).hexdigest()
    with tempfile.TemporaryDirectory() as tmp:
        cmd_unpack(argparse.Namespace(sdat=args.sdat, out=tmp))
        got = load(Path(tmp))
    if hashlib.sha1(got).hexdigest() != want:
        orig = src.read_bytes()
        first = next((i for i, (a, b) in enumerate(zip(orig, got)) if a != b), min(len(orig), len(got)))
        print(f"check: the packed archive differs from {args.sdat}: {len(got)} bytes against {len(orig)},"
              f" first difference at {first:#x}", file=sys.stderr)
        return 1
    print(f"check: packed archive matches {args.sdat} (sha1 {want})")
    return 0


def cmd_list(args) -> int:
    src = ROOT / args.sdat
    if not src.exists():
        sys.exit(f"error: {args.sdat} not found; run make extract")
    arc = Archive(src.read_bytes())
    pattern = re.compile(args.grep, re.I) if args.grep else None
    kinds = [args.kind] if args.kind else [k for k in KINDS if k in FILE_KINDS]
    print(f"{'kind':8} {'id':>4} {'name':32} {'bank':38} {'wave archives':20} {'size':>9}")
    for kind in kinds:
        for i, e in enumerate(arc.entries[kind]):
            name = arc.name(kind, i)
            if e is None or (pattern and not pattern.search(name)):
                continue
            bank = arcs = ""
            if kind == "seq":
                bank = f"{e['bankNo']} {arc.name('bank', e['bankNo'])}"
                b = arc.entries["bank"][e["bankNo"]] if e["bankNo"] < len(arc.entries["bank"]) else None
                arcs = " ".join(str(w) for w in b["waveArc"] if w is not None) if b else ""
            elif kind == "bank":
                arcs = " ".join(str(w) for w in e["waveArc"] if w is not None)
            size = len(arc.files[e["fileId"]]) if "fileId" in e else ""
            print(f"{kind:8} {i:4} {name:32} {bank:38} {arcs:20} {size:>9}")
    return 0


# ---------------------------------------------------------------- export

# Raise when the export's output changes for the same archive and tools.
EXPORT_VERSION = 1
# Sequences loop or stop on their own; the player cannot tell which, so each
# kind gets a fixed length (mm:ss) and fade (seconds). We considered working
# out each sequence's loop and end, or trimming silence after rendering, and
# chose not to: the files are what the sound engine plays, unedited. Measured
# on our archive, every sound effect that ends does so within 9.1 seconds,
# so their fade from 10 seconds on only reaches the 42 that loop.
LENGTHS = {"bgm": ("2:30", 10), "se": ("0:12", 2)}
FORMATS = {"mp3": ["-c:a", "libmp3lame", "-q:a", "2"], "ogg": ["-c:a", "libvorbis", "-q:a", "6"],
           "flac": ["-c:a", "flac"], "wav": []}
ALBUM = "Advance Wars: Dual Strike"
# The Sound Room's list (ov005): 8-byte cells of TextRef title, u16 song id,
# u8 CO picture, u8 kind (0 sequence, 1 stream). Titles are game text, found
# through gTextBanks (ov000): (first TextRef, string table) pairs.
OVERLAYS = Path("extract/arm9_overlays")
SOUND_ROOM = (5, 0x0236b10c, 67)
TEXT_BANKS = (0, 0x022f6bf0)


def overlay(n: int) -> tuple[bytes, int]:
    doc = yaml.safe_load((ROOT / OVERLAYS / "overlays.yaml").read_text())
    ov = next(o for o in doc["overlays"] if o["id"] == n)
    return (ROOT / OVERLAYS / ov["file_name"]).read_bytes(), ov["base_address"]


def sound_room_titles() -> tuple[dict[int, str], str | None]:
    """Sound Room title of each song id, and of the stream."""
    room, room_base = overlay(SOUND_ROOM[0])
    text, text_base = overlay(TEXT_BANKS[0])

    def word(addr: int) -> int:
        return u32(text, addr - text_base)

    def title(ref: int) -> str:
        bank = TEXT_BANKS[1]
        while word(bank + 4):
            if ref >= word(bank):
                return cstring(text, word(word(bank + 4) + 4 * (ref & 0xffffff)) - text_base).strip()
            bank += 8
        sys.exit(f"error: no text for {ref:#x}")

    songs, stream = {}, None
    for k in range(SOUND_ROOM[2]):
        ref, song, _, kind = struct.unpack_from("<IHBB", room, SOUND_ROOM[1] - room_base + 8 * k)
        if kind:
            stream = title(ref)
        else:
            songs[song] = title(ref)
    return songs, stream


def psf(reserved: bytes, program: bytes, tags: dict) -> bytes:
    """An NCSF file: a PSF file of version 0x25 whose program is an SDAT and
    whose reserved area holds the sequence number."""
    packed = zlib.compress(program, 9) if program else b""
    head = b"PSF\x25" + struct.pack("<III", len(reserved), len(packed), zlib.crc32(packed) if packed else 0)
    return head + reserved + packed + ("[TAG]" + "".join(f"{k}={v}\n" for k, v in tags.items())).encode()


def seqarc_sequences(ssar: bytes, where: str) -> list[tuple[bytes, dict] | None]:
    """Each sequence of a sequence archive as an SSEQ file and seq INFO entry.

    The sequences share one block of events and start at an offset in it. An
    SSEQ starts at its first byte, so each one gets the events from its start
    on. That is only right when nothing it runs jumps by offset (open track,
    jump, call); in our archive nothing does, and this stops if one does.
    """
    base, count = struct.unpack_from("<II", ssar, 0x18)
    events = ssar[base:]
    out = []
    for i in range(count):
        start, bank, volume, cprio, pprio, player = struct.unpack_from("<IHBBBB", ssar, 0x20 + 12 * i)
        if start == 0xffffffff:
            out.append(None)
            continue
        if offsets_used(events, start):
            sys.exit(f"error: {where} sequence {i} jumps by offset; export cannot cut it out")
        body = events[start:]
        size = 0x1c + len(body)
        sseq = b"SSEQ" + struct.pack("<HHIHH", 0xfeff, 0x100, size, 0x10, 1)
        sseq += b"DATA" + struct.pack("<II", size - 0x10, 0x1c) + body
        out.append((sseq, {"bankNo": bank, "volume": volume, "channelPrio": cprio, "playerPrio": pprio,
                           "playerNo": player}))
    return out


def offsets_used(events: bytes, start: int) -> bool:
    """Whether the events run from `start` hold an open track, jump or call."""
    def varlen(p: int) -> int:
        while events[p] & 0x80:
            p += 1
        return p + 1

    def args(p: int, cmd: int, last: str) -> int:
        """Position after cmd's arguments; `last` is how the last one is given:
        as usual, as a random range (0xa0 prefix) or as a variable (0xa1)."""
        if cmd < 0x80:
            p += 1                           # velocity, then the duration
            kind = "varlen"
        elif cmd in (0x80, 0x81):
            kind = "varlen"
        elif 0xb0 <= cmd <= 0xbd:
            p += 1                           # variable, then the value
            kind = 2
        elif 0xc0 <= cmd <= 0xd7:
            kind = 1
        elif cmd in (0xe0, 0xe1, 0xe3, 0xfe):
            kind = 2
        elif cmd in (0xfc, 0xfd, 0xff):
            return p
        else:
            sys.exit(f"error: sequence command {cmd:#x} not known")
        if last == "random":
            return p + 4
        if last == "variable":
            return p + 1
        return varlen(p) if kind == "varlen" else p + kind

    p = start
    while True:
        cmd = events[p]
        if cmd in (0x93, 0x94, 0x95):
            return True
        if cmd in (0xa0, 0xa1, 0xa2):
            p = args(p + 2, events[p + 1], {0xa0: "random", 0xa1: "variable", 0xa2: "usual"}[cmd])
        else:
            p = args(p + 1, cmd, "usual")
        if cmd in (0xfd, 0xff):
            return False


def safe_name(s: str) -> str:
    return re.sub(r'[/\\:*?"<>|]', "_", s)


def export_jobs(arc: Archive, ncsf: Path) -> tuple[bytes, list[dict]]:
    """The SDAT for the NCSF library, and one job per audio file.

    The library is our archive plus one sequence for each sequence inside a
    sequence archive, since an NCSF file names a sequence by number only.
    """
    titles, stream_title = sound_room_titles()
    entries = {k: list(v) for k, v in arc.entries.items()}
    names = {k: list(v) for k, v in arc.names.items()}
    files = list(arc.files)
    jobs = []
    for i, e in enumerate(arc.entries["seq"]):
        if e is None:
            continue
        name = arc.name("seq", i)
        if e["playerNo"] == 0:
            jobs.append({"seq": i, "kind": "bgm", "title": titles.get(i, name),
                         "path": f"bgm/{safe_name(titles.get(i, name))}"})
        else:
            jobs.append({"seq": i, "kind": "se", "title": name, "path": f"se/{i:03d}_{name}"})
    for a, e in enumerate(arc.entries["seqarc"]):
        if e is None:
            continue
        arc_name = arc.name("seqarc", a)
        subs = arc.subnames[a] or []
        for i, seq in enumerate(seqarc_sequences(arc.files[e["fileId"]], arc_name)):
            if seq is None:
                continue
            name = subs[i] if i < len(subs) and subs[i] else f"{arc_name}_{i:03d}"
            files.append(seq[0])
            entries["seq"].append({"fileId": len(files) - 1} | seq[1])
            names["seq"].append(name)
            jobs.append({"seq": len(entries["seq"]) - 1, "kind": "se", "title": name,
                         "path": f"se/{a:02d}_{arc_name}/{i:03d}_{name}"})
    for i, e in enumerate(arc.entries["strm"]):
        if e is not None:
            title = stream_title if i == 0 and stream_title else arc.name("strm", i)
            jobs.append({"strm": arc.files[e["fileId"]], "kind": "bgm", "title": title,
                         "path": f"bgm/{safe_name(title)}"})
    lib = build(names, arc.subnames, entries, files)
    for job in jobs:
        if "seq" in job:
            length, fade = LENGTHS[job["kind"]]
            src = ncsf / f"{job['seq']:03d}.minincsf"
            src.write_bytes(psf(struct.pack("<I", job["seq"]), b"",
                                {"_lib": "sound.ncsflib", "length": length, "fade": fade}))
        else:
            src = ncsf / f"{safe_name(job['title'])}.strm"
            src.write_bytes(job.pop("strm"))
        job["src"] = src
    return lib, jobs


def render(job: dict, out: Path, fmt: str) -> None:
    """One audio file: zxtune123 renders WAV, ffmpeg encodes it."""
    dest = out / f"{job['path']}.{fmt}"
    dest.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory() as tmp:
        subprocess.run([str(ROOT / ZXTUNE), "--wav", "filename=out", "--silent", str(job["src"])],
                       cwd=tmp, check=True, stdout=subprocess.DEVNULL)
        meta = ["-metadata", f"title={job['title']}", "-metadata", f"album={ALBUM}"]
        subprocess.run(["ffmpeg", "-loglevel", "error", "-y", "-i", f"{tmp}/out.wav", *FORMATS[fmt], *meta,
                        str(dest)], check=True)


def linked_worktree() -> bool:
    git = subprocess.run(["git", "rev-parse", "--git-dir", "--git-common-dir"], cwd=ROOT,
                         capture_output=True, text=True)
    dirs = git.stdout.split()
    return git.returncode == 0 and len(dirs) == 2 and Path(ROOT, dirs[0]).resolve() != Path(ROOT, dirs[1]).resolve()


def cmd_export(args) -> int:
    if not args.auto:
        return export(args)
    # make assets runs this on every make release; the music is not part of
    # the build, so nothing here may stop the release.
    if linked_worktree():
        print("export: skipped in a linked worktree; run make music to export here")
        return 0
    if not shutil.which("ffmpeg"):
        print("export: skipped, ffmpeg not found (sudo apt install ffmpeg; then make music)")
        return 0
    try:
        return export(args)
    except (SystemExit, OSError, subprocess.CalledProcessError) as e:
        print(f"export: failed ({e}); make music shows the error. The build does not need the music.")
        return 0


def export(args) -> int:
    if not shutil.which("ffmpeg"):
        sys.exit("error: ffmpeg not found; sudo apt install ffmpeg")
    if not (ROOT / ZXTUNE).exists():
        sys.exit(f"error: {ZXTUNE} not found; run make setup")
    src = ROOT / args.sdat
    if not src.exists():
        sys.exit(f"error: {args.sdat} not found; run make extract")
    data = src.read_bytes()
    out = ROOT / args.out
    stamp = out / ".stamp"
    key = hashlib.sha1(data)
    for n in (SOUND_ROOM[0], TEXT_BANKS[0]):
        key.update(overlay(n)[0])
    key.update((ROOT / ZXTUNE).parent.parent.joinpath(".sha256").read_bytes())
    key.update(f"{EXPORT_VERSION} {args.format}".encode())
    if not args.force and stamp.exists() and stamp.read_text().strip() == key.hexdigest():
        print(f"export: {args.out} is up to date")
        return 0

    ncsf = ROOT / NCSF_DIR
    shutil.rmtree(ncsf, ignore_errors=True)
    ncsf.mkdir(parents=True)
    lib, jobs = export_jobs(Archive(data), ncsf)
    (ncsf / "sound.ncsflib").write_bytes(psf(b"", lib, {}))
    shutil.rmtree(out, ignore_errors=True)
    out.mkdir(parents=True)
    # zxtune123 uses about two cores per song.
    workers = args.jobs or max(1, (os.cpu_count() or 2) // 2)
    print(f"export: rendering {len(jobs)} files with {workers} jobs into {args.out}")
    with concurrent.futures.ThreadPoolExecutor(workers) as pool:
        for f in [pool.submit(render, job, out, args.format) for job in jobs]:
            f.result()
    stamp.write_text(key.hexdigest() + "\n")
    print(f"export: {len(jobs)} files in {args.out}")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = parser.add_subparsers(required=True)
    p = sub.add_parser("unpack", help="write the archive's files and sound.yaml")
    p.add_argument("--sdat", default=str(SDAT))
    p.add_argument("--out", default=str(EXTRACTED))
    p.set_defaults(func=cmd_unpack)
    p = sub.add_parser("pack", help="rebuild the archive from the unpacked files")
    p.add_argument("--in", dest="input", default=str(EXTRACTED))
    p.add_argument("--out", default=str(PACKED))
    p.set_defaults(func=cmd_pack)
    p = sub.add_parser("check", help="unpack and pack again; compare with the ROM's archive")
    p.add_argument("--sdat", default=str(SDAT))
    p.set_defaults(func=cmd_check)
    p = sub.add_parser("list", help="the archive's entries, by kind")
    p.add_argument("--sdat", default=str(SDAT))
    p.add_argument("--kind", choices=KINDS)
    p.add_argument("--grep", help="regular expression on the name, case ignored")
    p.set_defaults(func=cmd_list)
    p = sub.add_parser("export", help="render every song, sound effect and stream to audio files")
    p.add_argument("--sdat", default=str(SDAT))
    p.add_argument("--out", default=str(MUSIC))
    p.add_argument("--format", choices=FORMATS, default="mp3")
    p.add_argument("--jobs", type=int, help="songs rendered at once (default: half the CPUs)")
    p.add_argument("--force", action="store_true", help="export even when the files are up to date")
    p.add_argument("--auto", action="store_true",
                   help="for make assets: skip in a linked worktree or without ffmpeg; never fail")
    p.set_defaults(func=cmd_export)
    args = parser.parse_args()
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
