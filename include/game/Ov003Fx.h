#ifndef GAME_OV003_FX_H
#define GAME_OV003_FX_H

#include "types.h"

/* Overlay 3 state through data_ov003_02364a84: palette colours, tile copy
 * requests that the two frame callbacks (one per screen) carry out, and the
 * animation counters Ov003_StepAnims steps on Ov003State.frameCount. A
 * request is a 2-bit counter: nonzero means a copy is pending. */
typedef struct Ov003Fx {
  u32 unk_00;
  u32 colors;      /* two 16-bit colours */
  u8 mode;         /* frameCount / 4, mod 16; 0 or 11 re-sends the colours */
  u8 unk_09_0 : 2; /* set to 1 when unk_09_2 changes */
  u8 unk_09_2 : 6; /* 0-3 on a 100-frame cycle; reef sprite frame */
  u8 unk_0a_0 : 2; /* set to 1 when unk_0a_2 changes */
  u8 unk_0a_2 : 6; /* 0-7, one step every 14 frames */
  u8 unk_0b_0 : 2; /* set to 1 when unk_0b_2 changes */
  u8 unk_0b_2 : 6; /* 0-3, one step every 20 frames; base sprite frame */
  u8 unk_0c_0 : 4;
  u8 unk_0c_4 : 4;
} Ov003Fx;

extern Ov003Fx* data_ov003_02364a84;

extern Ov003Fx data_ov003_02365d00; /* what data_ov003_02364a84 points to */

#endif
