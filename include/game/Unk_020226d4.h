#ifndef GAME_UNK_020226D4_H
#define GAME_UNK_020226D4_H

#include "types.h"

/* Argument of func_020226d4 and func_02021bac (no direct callers, likely
 * task callbacks). unk_50 goes to SpriteAnim_Free, which clears the first
 * word of what it points to. */
typedef struct Unk_020226d4 {
  u8 pad_00[0x50];
  void* unk_50;
} Unk_020226d4;

#endif
