#ifndef GAME_UNK_OV003_02364A84_H
#define GAME_UNK_OV003_02364A84_H

#include "types.h"

/* Overlay 3 object reached through data_ov003_02364a84. */
typedef struct Unk_ov003_02364a84 {
  u8 pad_00[0x09];
  u8 unk_09_0 : 2;
  u8 unk_09_2 : 6; /* reef sprite frame */
  u8 unk_0a_0 : 2;
  u8 unk_0a_2 : 6;
  u8 unk_0b_0 : 2;
  u8 unk_0b_2 : 6; /* base sprite frame */
  u8 unk_0c_0 : 4;
  u8 unk_0c_4 : 4;
} Unk_ov003_02364a84;

extern Unk_ov003_02364a84* data_ov003_02364a84;

#endif
