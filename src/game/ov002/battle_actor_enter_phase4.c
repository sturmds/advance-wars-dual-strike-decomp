#include "types.h"
#include "game/battle_actor.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

/* Case 4 of the six-way switch in BattleCellTask_RunStep (the name is a guess).
 * Sets unk_54 to 12 when the other side's fighter has 2 at unk_49 for its
 * current pose, else 1. */
void BattleActor_EnterPhase4(BattleActor *actor)
{
    int other;

    actor->unk_59_2 = 1;
    other = (actor->side + 1) & 1;
    if (BATTLE_FIGHTER(other)->unk_49[Ov002_gBattleScene->sides[other].usesSecondary] == 2) {
        actor->unk_54 = 12;
    } else {
        actor->unk_54 = 1;
    }
    actor->unk_55 = 0;
}
