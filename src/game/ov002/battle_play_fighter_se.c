#include "types.h"
#include "game/menu_state.h"
#include "game/battle_fighter_sprites.h"

void Sound_PlaySe(u32 soundId);
void Battle_PlayDefaultFighterSe(int side);

/* Plays the fighter's own sound for this event, or a default one picked by
 * its unit type when the fighter has none. The inner check can never be true;
 * the original has it. */
void Battle_PlayFighterSe(int side) {
  u32 sound = BATTLE_SIDE_DATA(side)->sounds->unk_14;

  if (sound != BATTLE_SOUND_NONE) {
    if (sound == BATTLE_SOUND_NONE) {
      return;
    }
    Sound_PlaySe(sound);
  } else {
    Battle_PlayDefaultFighterSe(side);
  }
}
