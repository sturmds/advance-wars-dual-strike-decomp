#ifndef GAME_OV003_FX_H
#define GAME_OV003_FX_H

#include "types.h"

/* Overlay 3 state through data_ov003_02364a84: palette colours and tile
 * copy requests that the two frame callbacks (one per screen) carry out.
 * A request is a 2-bit counter: nonzero means a copy is pending. */
typedef struct Ov003Fx {
  u32 unk_00;
  u32 colors; /* two 16-bit colours */
  u8 mode;    /* 0 or 11 re-sends the colours */
  u8 unk_09_0 : 2;
  u8 unk_09_2 : 6;
  u8 unk_0a_0 : 2;
  u8 unk_0a_2 : 6;
  u8 unk_0b_0 : 2;
  u8 unk_0b_2 : 6;
} Ov003Fx;

extern Ov003Fx* data_ov003_02364a84;

#endif
