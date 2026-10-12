#ifndef GAME_UNK_OV005_0236BB84_H
#define GAME_UNK_OV005_0236BB84_H

#include "types.h"

struct Task;

/* Overlay 5 work block (0x24 bytes) that func_ov005_023551f8 allocates
 * into Ov005Task.work and data_ov005_0236bb84. */
typedef struct Unk_ov005_0236bb84 {
  u8 pad_00[0x4];
  u32 unk_04;
  int unk_08;
  u8 pad_0c[0x10 - 0x0c];
  struct Task*
      fadeTask; /* from the BrightnessFade_StartOutBlackScreen0 family */
  struct Task* fadeTask2;
  struct Task* songFadeTask; /* func_020fcb08 */
  u32 unk_1c;
  int unk_20; /* set to 2000 by Ov005_SetupMainScreenLayers2And3 */
} Unk_ov005_0236bb84;

#endif
