#include "types.h"
#include "game/battle_scene.h"
#include "game/menu_state.h"

extern MenuState gMenuState;

int Battle_GetAirGroundMatchup(int side);

/* Needed to match: every case reads the entry again. */
#define ENTRY(side) (BATTLE_SIDE_ANIM(side)->unk_08[Ov002_gBattleScene->sides[side].usesSecondary])

/* Like Battle_GetMatchupNibble0, for the side's second list of entries. */
int Battle_GetMatchupNibble1(int side)
{
    switch (Battle_GetAirGroundMatchup(side)) {
    case 0:
        return ENTRY(side)->unk_29_4;
    case 1:
        return ENTRY(side)->unk_2a_0;
    case 2:
        return ENTRY(side)->unk_2a_4;
    default:
        return ENTRY(side)->unk_29_4;
    }
}
