#include "types.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_PlaySe(u32 soundId);
void func_ov002_0235fa50(int side);

/* Plays the fighter's own sound for this event, or a default one picked by
 * its unit type when the fighter has none. The inner check can never be true;
 * the original has it. */
void Battle_PlayFighterSe(int side)
{
    u32 sound = BATTLE_FIGHTER(side)->sounds->unk_14;

    if (sound != BATTLE_SOUND_NONE) {
        if (sound == BATTLE_SOUND_NONE) {
            return;
        }
        Sound_PlaySe(sound);
    } else {
        func_ov002_0235fa50(side);
    }
}
