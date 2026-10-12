#ifndef GAME_UNK_OV005_0235A30C_H
#define GAME_UNK_OV005_0235A30C_H

#include "types.h"
#include "game/Unk_ov005_0235abfc.h"

/* Overlay 5 work block of the task step Ov005_UpdateStaffCreditsScreen
 * (Ov005Task.work). At least 0x834c bytes. */
typedef struct Unk_ov005_0235a30c {
  u32 skippedStep; /* task step at the moment the player skipped */
  struct Task* fadeTask;
  struct Task* fadeTask2;
  u8 pad_000c[0x8010 - 0x000c];
  u32 pageIndex;  /* page of data_ov005_0236a418 on screen */
  int pageFrames; /* frames the page is shown, from the page table */
  Unk_ov005_0235abfc entriesA[17];
  Unk_ov005_0235abfc entriesB[17];
  void* unk_8348; /* passed to Task_Destroy (task system) */
} Unk_ov005_0235a30c;

#endif
