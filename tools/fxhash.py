"""64-bit FxHash of a byte string, as dsd computes it for config.yaml `hash:` fields.

Rust's `fxhash::hash64(&[u8])` hashes the slice length first, then the
bytes in 8, 4, 2 and 1 byte little-endian words.
"""

import struct

_K = 0x517CC1B727220A95
_MASK = (1 << 64) - 1


def _add(h: int, word: int) -> int:
    return ((((h << 5) | (h >> 59)) & _MASK ^ word) * _K) & _MASK


def fxhash64(data: bytes) -> int:
    n = len(data)
    h = _add(0, n)
    i = 0
    while n - i >= 8:
        h = _add(h, struct.unpack_from("<Q", data, i)[0])
        i += 8
    if n - i >= 4:
        h = _add(h, struct.unpack_from("<I", data, i)[0])
        i += 4
    if n - i >= 2:
        h = _add(h, struct.unpack_from("<H", data, i)[0])
        i += 2
    if n - i >= 1:
        h = _add(h, data[i])
    return h
