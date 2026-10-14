#ifndef GAME_BATTLE_FIGHTER_H
#define GAME_BATTLE_FIGHTER_H

#include "types.h"

#define BATTLE_SOUND_NONE 0xffff /* an unused entry of BattleSounds */

/* Sound effect ids (Sound_PlaySe) of one fighter on the battle animation screen. */
typedef struct BattleSounds {
  u32 unk_00; /* Sound_PlaySubSe id of the attacker (Battle_StartAttackerMoveSound) */
  u32 unk_04; /* Battle_PlayFighterStartSe, BattleSide.usesSecondary of 0 */
  u32 unk_08; /* Battle_PlayFighterStartSe, otherwise */
  u32 unk_0c; /* Battle_PlayFighterReactSe, other fighter's state not 2 */
  u32 unk_10; /* Battle_PlayFighterReactSe, other fighter's state 2 */
  u32 unk_14; /* Battle_PlayFighterSe; without it Battle_PlayDefaultFighterSe */
} BattleSounds;

typedef struct BattlePoint {
  s16 x;
  s16 y;
} BattlePoint;

/* Task of the ov002 battle animation that Battle_ReleaseSideTasks counts frames on. */
typedef struct BattleCounterTask {
  u8 pad_00[0x3c];
  s16 unk_3c;
  u8 pad_3e[0x42 - 0x3e];
  s16 unk_42;
  u8 pad_44[0x5d - 0x44];
  u8 unk_5d; /* zero: the task is done */
} BattleCounterTask;

#endif
