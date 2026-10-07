#!/usr/bin/env python3
"""Set the secure-area CRC and header CRC in a ROM built by `dsd rom build`.

dsd can only compute the secure-area CRC (header 0x6C) with an ARM7 BIOS
dump, which this project does not use. The CRC covers the encrypted secure
area, so when our secure area (0x4000-0x7FFF) equals the original's, the
original CRC applies. This script checks that, copies the CRC, and
recomputes the header CRC (0x15E). It changes nothing else.

Usage: fix_header.py IN.nds ORIG.nds OUT.nds
"""

import struct
import sys

SECURE_AREA = slice(0x4000, 0x8000)
SECURE_CRC = slice(0x6C, 0x6E)
HEADER_CRC = 0x15E


def crc16(data: bytes) -> int:
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for _ in range(8):
            crc = (crc >> 1) ^ 0xA001 if crc & 1 else crc >> 1
    return crc


def main() -> int:
    src, orig_path, dst = sys.argv[1:4]
    rom = bytearray(open(src, "rb").read())
    with open(orig_path, "rb") as f:
        orig = f.read(0x8000)
    if rom[SECURE_AREA] != orig[SECURE_AREA]:
        sys.exit(f"error: the secure area of {src} differs from {orig_path}, "
                 "so the original secure-area CRC does not apply")
    rom[SECURE_CRC] = orig[SECURE_CRC]
    struct.pack_into("<H", rom, HEADER_CRC, crc16(rom[:HEADER_CRC]))
    with open(dst, "wb") as f:
        f.write(rom)
    return 0


if __name__ == "__main__":
    sys.exit(main())
