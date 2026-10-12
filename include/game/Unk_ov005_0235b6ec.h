#ifndef GAME_UNK_OV005_0235B6EC_H
#define GAME_UNK_OV005_0235B6EC_H

#include "types.h"

/* One 0x18-byte entry of a list the screen at Ov005_UpdateStaffCreditsScreen draws;
 * a list ends at the first entry whose unk_00 is 0. */
typedef struct Unk_ov005_0235b6ec_Entry {
  u16 unk_00;
  u16 charCount; /* 8 pixels wide each; the entry is centered on the 256-pixel screen */
  u8 pad_04[0x0e - 0x04];
  s16 x;
  s16 y;
  u8 pad_12[0x18 - 0x12];
} Unk_ov005_0235b6ec_Entry;

/* Work block of the screen at Ov005_UpdateStaffCreditsScreen (task->work). */
typedef struct Unk_ov005_0235b6ec {
  u8 pad_0000[0x8018];
  Unk_ov005_0235b6ec_Entry templates[17];
  Unk_ov005_0235b6ec_Entry entries[17];
} Unk_ov005_0235b6ec;

#endif
