#include "sdk/mi.h"

void Unpack_HuffmanToWords(const u8 *src, u32 *dest)
{
    const u8 *treeRoot;
    const u8 *node;
    const u8 *data;
    u32 bitSize;
    u32 pending;
    u32 count;
    u32 perWord;
    int remaining;
    u32 bits;
    int i;
    u32 bit;
    u32 flags;

    treeRoot = src + 5;
    bitSize = *src & 15;
    pending = 0;
    count = 0;
    perWord = (bitSize & 7) + 4;
    remaining = *(const u32 *)src >> 8;
    data = src + 4 + (src[4] + 1) * 2;
    node = treeRoot;
    while (remaining > 0) {
        bits = *(const u32 *)data;
        data += 4;
        for (i = 32; --i >= 0;) {
            bit = (bits >> 31) & 1;
            flags = *node << bit;
            node = (const u8 *)(((u32)node >> 1) << 1) + ((*node & 63) + 1) * 2 + bit;
            if (flags & 0x80) {
                pending >>= bitSize;
                pending |= *node << (32 - bitSize);
                node = treeRoot;
                count++;
                if (count == perWord) {
                    *dest++ = pending;
                    remaining -= 4;
                    count = 0;
                }
            }
            if (remaining > 0) {
                bits <<= 1;
            } else {
                break;
            }
        }
    }
}
