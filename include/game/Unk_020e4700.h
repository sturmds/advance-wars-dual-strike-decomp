#ifndef GAME_UNK_020E4700_H
#define GAME_UNK_020E4700_H

#include "types.h"

/* Passed to func_020e4700 (no direct callers; probably a task callback).
 * unk_2c and unk_30 look like map x and y (func_020e462c). */
typedef struct Unk_020e4700 {
  u8 pad_00[0x2c];
  int unk_2c;
  int unk_30;
} Unk_020e4700;

#endif
