#include "types.h"
#include "game/unit_stats.h"

BOOL func_020985b0(int mask);
BOOL func_02095bc0(int siteType);
int func_02095fb8(void);
int Ai_CountUnitsOfType(int type);

#define AI_PROFILE_BLACK_BOMBS 2
#define SITE_TYPE_BLACK_BOMB   2

/* True when the AI profile allows Black Bombs (bit 2 of the byte func_020985b0
 * tests), the player has no Black Bomb, and a target is ready: with a
 * unitType, func_02095fb8 finds a build site and the player has a unit of that
 * type; with 0, a site of type 2 exists. */
BOOL AiBuild_WantsBlackBombInstead(int unitType)
{
    if (!func_020985b0(AI_PROFILE_BLACK_BOMBS)) {
        return FALSE;
    }
    if (unitType != 0) {
        if (func_02095fb8() == -1) {
            return FALSE;
        }
    } else if (!func_02095bc0(SITE_TYPE_BLACK_BOMB)) {
        return FALSE;
    }
    if (Ai_CountUnitsOfType(UNIT_BLACK_BOMB) != 0) {
        return FALSE;
    }
    if (unitType != 0 && Ai_CountUnitsOfType(unitType) == 0) {
        return FALSE;
    }
    return TRUE;
}
