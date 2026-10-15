#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/battle_fighter_sprites.h"

void Sound_PlaySe(u32 soundId);
void Battle_FadeAttackerSubSe(int side);

/* Fades out the attacker's looping sound, then plays the fighter's sound for
 * BattleSide.usesSecondary of 0 or not 0. The name is a guess. The two checks of
 * the sound id are written out twice, as in the original. */
void Battle_PlayFighterStartSe(int side) {
  u32 sound;

  Battle_FadeAttackerSubSe(side);
  if (Ov002_gBattleScene->sides[side].usesSecondary == 0) {
    sound = BATTLE_SIDE_DATA(side)->sounds->unk_04;
    if (sound == BATTLE_SOUND_NONE) {
      return;
    }
    Sound_PlaySe(sound);
  } else {
    sound = BATTLE_SIDE_DATA(side)->sounds->unk_08;
    if (sound == BATTLE_SOUND_NONE) {
      return;
    }
    Sound_PlaySe(sound);
  }
}
