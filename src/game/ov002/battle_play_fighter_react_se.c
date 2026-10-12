#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"
#include "game/battle_fighter_sprites.h"

extern MenuState gMenuState;

void Sound_PlaySe(u32 soundId);

/* Plays the fighter's sound unk_10 when the other side's fighter has state 2
 * (BattleSideData.unk_49 at its BattleSide.usesSecondary), else unk_0c. The name is a
 * guess. */
void Battle_PlayFighterReactSe(int side) {
  u32 sound;
  int other = (side + 1) & 1;

  if (BATTLE_SIDE_DATA(other)
          ->unk_49[Ov002_gBattleScene->sides[other].usesSecondary] == 2) {
    sound = BATTLE_SIDE_DATA(side)->sounds->unk_10;
    if (sound == BATTLE_SOUND_NONE) {
      return;
    }
    Sound_PlaySe(sound);
  } else {
    sound = BATTLE_SIDE_DATA(side)->sounds->unk_0c;
    if (sound == BATTLE_SOUND_NONE) {
      return;
    }
    Sound_PlaySe(sound);
  }
}
