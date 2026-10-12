#include "types.h"

#define MAIN_RAM_START 0x02000000
#define MAIN_RAM_END 0x02400000

typedef void (*UnpackFunc)(const void* src, void* dst);

void Mem_Copy32(const void* src, void* dst,
                u32 size); /* copies in 32-bit units */

/* Two unpackers per compression type (header bits 4-7): [0] when dst is
 * outside main RAM (VRAM), [1] for main RAM. Type 0 (raw) has none. */
extern UnpackFunc gUnpackFuncs[];

/* Unpacks a block with a DS compression header to dst, choosing the
 * unpacker by compression type and by whether dst is in main RAM. Raw
 * blocks are copied as they are. */
void Data_Unpack(const void* src, void* dst) {
  int inMainRam;
  u8 header;
  UnpackFunc unpack;

  if ((u32)dst >= MAIN_RAM_START && (u32)dst <= MAIN_RAM_END) {
    inMainRam = 1;
  } else {
    inMainRam = 0;
  }
  header = *(const u8*)src;
  unpack = gUnpackFuncs[((header & 0xf0) >> 3) + inMainRam];
  if (unpack == NULL) {
    /* raw: copy (size field * 16) | low nibble bytes */
    Mem_Copy32(src, dst, (header & 0xf) | ((*(const u32*)src & ~0xff) >> 4));
    return;
  }
  /* needed to match: the original tests unpack a second time */
  if (unpack != NULL) {
    unpack(src, dst);
  }
}
