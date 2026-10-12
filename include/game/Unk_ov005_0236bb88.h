#ifndef GAME_UNK_OV005_0236BB88_H
#define GAME_UNK_OV005_0236BB88_H

#include "types.h"

struct Task;

/* Overlay 5 work block (0x24 bytes) that func_ov005_02357804 allocates
 * into Ov005Task.work and data_ov005_0236bb88. */
typedef struct Unk_ov005_0236bb88 {
  u8 pad_00[0x8];
  u32 unk_08;
  u8 pad_0c[0x18 - 0x0c];
  struct Task* songFadeTask; /* Sound_StartSongSwitch */
  u8 pad_1c[0x24 - 0x1c];
} Unk_ov005_0236bb88;

#endif
