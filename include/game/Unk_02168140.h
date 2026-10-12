#ifndef GAME_UNK_02168140_H
#define GAME_UNK_02168140_H

#include "types.h"

/* Task that runs the script data_02168140. The creator fills three
 * arguments after Task_Create or Task_CreateChild (TileListTask_StartChild takes them
 * as parameters). What the script does with them is not known yet. */
typedef struct Unk_02168140Task {
  u8 pad_00[0x2c];
  int unk_2c;
  int unk_30;
  u8 pad_34[0x4c - 0x34];
  void* unk_4c;
} Unk_02168140Task;

#endif
