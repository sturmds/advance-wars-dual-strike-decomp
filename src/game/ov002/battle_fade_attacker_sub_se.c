#include "types.h"
#include "game/battle_scene.h"

void Sound_StopSubSe(int fadeFrames);

/* Fades out the attacker's looping sound (sub sound effect) over 10 frames when the side is the attacker. */
void Battle_FadeAttackerSubSe(int side)
{
    if (side == Ov002_gBattleScene->attackerSide) {
        Sound_StopSubSe(10);
    }
}
