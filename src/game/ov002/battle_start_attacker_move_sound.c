#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/battle_fighter_sprites.h"

void Sound_PlaySubSe(u32 soundId);
BOOL Battle_IsSideFootUnit(int side);
void BattleFootstepTask_Start(int side);

/* For the attacking side: starts the footstep task if the side is a foot unit,
 * otherwise plays the unit's looping sound (sub sound effect). */
void Battle_StartAttackerMoveSound(int side) {
  u32 sound;

  if (side != Ov002_gBattleScene->attackerSide) {
    return;
  }
  if (Battle_IsSideFootUnit(side) == TRUE) {
    BattleFootstepTask_Start(side);
    return;
  }
  sound = BATTLE_SIDE_DATA(side)->sounds->unk_00;
  if (sound != BATTLE_SOUND_NONE) {
    Sound_PlaySubSe(sound);
  }
}
