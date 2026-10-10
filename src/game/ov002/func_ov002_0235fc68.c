#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

void Sound_PlaySubSe(u32 soundId);
int Battle_IsSideFootUnit(void);
void BattleFootstepTask_Start(int side);

/* Starts the attacker's looping sound (sub sound effect) unless
 * Battle_IsSideFootUnit says otherwise. */
void func_ov002_0235fc68(int side)
{
    u32 sound;

    if (side != Ov002_gBattleScene->attackerSide) {
        return;
    }
    if (Battle_IsSideFootUnit() == 1) {
        BattleFootstepTask_Start(side);
        return;
    }
    sound = BATTLE_FIGHTER(side)->sounds->unk_00;
    if (sound != BATTLE_SOUND_NONE) {
        Sound_PlaySubSe(sound);
    }
}
