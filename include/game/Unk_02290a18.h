#ifndef GAME_UNK_02290A18_H
#define GAME_UNK_02290A18_H

#include "types.h"

/* data_02290a18 (main bss), set up by Save_ResetProgress. The main menu reads
 * unk_78 and unk_7a as scores (Score_ToRank turns them into a rank) for
 * page 11 entry kinds 25 and 26. */
typedef struct Unk_02290a18 {
  u8 pad_00[0x78];
  u16 unk_78;
  u16 unk_7a;
} Unk_02290a18;

extern Unk_02290a18 data_02290a18;

#endif
