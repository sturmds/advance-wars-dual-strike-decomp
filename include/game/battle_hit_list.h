#ifndef GAME_BATTLE_HIT_LIST_H
#define GAME_BATTLE_HIT_LIST_H

#include "types.h"

/* One byte of flags per entry of a BattleHitList. */
typedef struct BattleHitFlags {
  u8 cell : 3; /* index into the side's BattleEntry list */
  u8 unk_3 : 3;
  u8 unk_6 : 1;
  u8 unk_7 : 1;
} BattleHitFlags;

/* A list of up to five entries of the ov002 battle animation, kept sorted by key
 * (the name is a guess). */
typedef struct BattleHitList {
  BattleHitFlags flags[6];
  u16 key[5];
  u8 cursor; /* next entry BattleCells_RunSequence runs */
  u8 count;
} BattleHitList;

/* Layout of the 20-byte drain profile (BattleScreenWork.drainProfile) that the keys of a
 * BattleHitList come from (Battle_BuildHitLists). */
typedef struct BattleHitTimes {
  u8 pad_00[4];
  u16 unk_04; /* added to every key */
  u16 unk_06; /* added to the keys of the other side's step list */
  u16 unk_08;
  u8 unk_0a[5]; /* key offsets of the attacking side */
  u8 unk_0f[5]; /* key offsets of the other side */
} BattleHitTimes;

#endif
