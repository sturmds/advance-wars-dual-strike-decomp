#ifndef GAME_OV003_ANIM_H
#define GAME_OV003_ANIM_H

#include "types.h"

/* Overlay 3 animation counters, through data_ov003_02364a84. They step on
 * Ov003State.frameCount (Ov003_StepAnims); what they animate is not known. */
typedef struct Ov003Anim {
  u8 pad_00[0x08];
  u8 unk_08;       /* frameCount / 4, mod 16 */
  u8 unk_09_0 : 2; /* set to 1 when unk_09_2 changes */
  u8 unk_09_2 : 6; /* 0-3 on a 100-frame cycle */
  u8 unk_0a_0 : 2; /* set to 1 when unk_0a_2 changes */
  u8 unk_0a_2 : 6; /* 0-7, one step every 14 frames */
  u8 unk_0b_0 : 2; /* set to 1 when unk_0b_2 changes */
  u8 unk_0b_2 : 6; /* 0-3, one step every 20 frames */
  u8 unk_0c_0 : 4;
  u8 unk_0c_4 : 4;
} Ov003Anim;

extern Ov003Anim* data_ov003_02364a84;

#endif
