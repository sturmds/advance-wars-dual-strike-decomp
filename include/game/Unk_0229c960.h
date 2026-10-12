#ifndef GAME_UNK_0229C960_H
#define GAME_UNK_0229C960_H

#include "types.h"

/* data_0229c960 (main bss). */
typedef struct Unk_0229c960 {
  void* userGameInfo;     /* Wireless_Init points it at data_0229c910 */
  u16 userGameInfoLength; /* 24 */
  u16 unk_06;
  u32 ggid;      /* compared with a scanned parent's game group id */
  u16 tgid;      /* temporary group id, changes for every parent start */
  u16 entryFlag; /* 1: children may join */
  u16 unk_10;
  u16 multiBootFlag;
  u16 ksFlag;
  u16 csFlag;
  u16 beaconPeriod; /* Wireless_Init: 180 plus a value derived from the console's MAC address */
  u8 pad_1a[0x32 - 0x1a];
  u16 channel;
  u16 parentMaxSize; /* largest packet a parent sends */
  u16 childMaxSize;  /* largest packet a child sends */
  u8 pad_38[0x40 - 0x38];
} Unk_0229c960;

extern Unk_0229c960 data_0229c960;

#endif
