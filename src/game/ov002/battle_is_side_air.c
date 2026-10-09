#include "types.h"
#include "game/battle_scene.h"
#include "game/unit_stats.h"

/* Needed to match: the unit type of a side sits at scene offset 3 + 12 * side,
 * one byte before what BattleScene.sides[side] starts at. */
#define BATTLE_SIDE_UNIT_TYPE(side) (((u8 *)Ov002_gBattleScene)[3 + (side) * sizeof(BattleSide)])

/* Nonzero when the unit on that side is a plane or helicopter. 31 is not a
 * unit type in Ov000_gUnitStats; the value is set elsewhere in ov002. */
BOOL Battle_IsSideAir(int side)
{
    switch (BATTLE_SIDE_UNIT_TYPE(side)) {
    case UNIT_STEALTH:
    case UNIT_BLACK_BOMB:
    case UNIT_FIGHTER:
    case UNIT_BOMBER:
    case UNIT_B_COPTER:
    case UNIT_T_COPTER:
    case 31:
        return TRUE;
    default:
        return FALSE;
    }
}
