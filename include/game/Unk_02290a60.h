#ifndef GAME_UNK_02290A60_H
#define GAME_UNK_02290A60_H

#include "types.h"

/* Three 0x10-byte save records at data_02290a60 (main bss), cleared by
 * func_0210408c. What they hold is not known yet. */
typedef struct Unk_02290a60 {
  u8 unk_00;
  u8 unk_01;
  u8 unk_02;
  u8 unk_03;
  u8 unk_04;
  u8 unk_05;
  u16 unk_06;
  u16 unk_08;
  u16 unk_0a;
  u16 unk_0c;
  u16 unk_0e;
} Unk_02290a60;

extern Unk_02290a60 data_02290a60[];

#endif
