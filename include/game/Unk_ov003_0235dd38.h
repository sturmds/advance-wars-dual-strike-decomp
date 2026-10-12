#ifndef GAME_UNK_OV003_0235DD38_H
#define GAME_UNK_OV003_0235DD38_H

#include "types.h"

/* Overlay 3 object passed to func_ov003_0235dd38 and func_ov003_02363ba4,
 * which both clear its halfword at 0x4c. */
typedef struct Unk_ov003_0235dd38 {
  u8 pad_00[0x2c];
  s32 slideX; /* banner sprite x offset, eased by the letter steps */
  u8 pad_30[0x4c - 0x30];
  union {
    u16 unk_4c;
    s16 frame; /* counts up to a fade length */
  };
  u8 pad_4e[0x52 - 0x4e];
  u16 unk_52; /* row number, 1 to 5 */
  u8 pad_54[0x58 - 0x54];
  int count; /* how many of the 16 slots are filled (func_ov003_0235ce7c) */
  u8 pad_5c[0x64 - 0x5c];
  u16 unk_64;
} Unk_ov003_0235dd38;

#endif
