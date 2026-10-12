#ifndef GAME_UNK_0229C910_H
#define GAME_UNK_0229C910_H

#include "types.h"

/* data_0229c910 (main bss, 0x18 bytes). Link play. */
typedef struct Unk_0229c910 {
  u8 pad_00[0x14];
  u16 usedAids; /* bit 0 (parent) plus each connected child aid; func_0207a750 sets 1 */
  u8 maxChildren; /* copy of Unk_0229c960.unk_10; Wireless_AddParent skips a parent whose aids 1 to maxChildren are all used */
  u8 unk_17;      /* copy of Unk_0215fe84.unk_35 */
} Unk_0229c910;

extern Unk_0229c910 data_0229c910;

#endif
