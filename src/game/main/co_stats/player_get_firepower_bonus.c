#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

int Player_GetCoFirepower(int playerIndex, int slot, int powerState, int unitType);
int func_020e5508(int playerIndex, BOOL tagPower); /* tag partner firepower bonus, percent */

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define PLAYER(i) (data_027c027c->unk_48[data_027c0284]->players[i])

/* Player.unk_25: 1 front, 2 back CO half of a tag power */
static inline BOOL Player_InTagPower(int playerIndex)
{
    return PLAYER(playerIndex).unk_25 == 1 || PLAYER(playerIndex).unk_25 == 2;
}

static inline BOOL Player_TagPowerOn(int playerIndex)
{
    BOOL on = FALSE;

    if (PLAYER(playerIndex).back.id != 0 && Player_InTagPower(playerIndex)) {
        on = TRUE;
    }
    return on;
}

/* Firepower bonus in percent of a player's unit: the CO's bonus, the tag
 * partner's bonus during a tag power, and the flat bonus while a power is
 * on. 0 with coEffectsOn clear, and for Oozium (UNIT_CLASS_ALL). */
int Player_GetFirepowerBonus(int playerIndex, int slot, int powerState, int unitType)
{
    int bonus = 0;

    if (!data_027c027c->unk_48[data_027c0284]->coEffectsOn) {
        return bonus;
    }
    if (Ov000_gUnitStats[unitType].unitClass == UNIT_CLASS_ALL) {
        return bonus;
    }
    bonus += Player_GetCoFirepower(playerIndex, slot, powerState, unitType);
    if (Player_TagPowerOn(playerIndex)) {
        bonus += func_020e5508(playerIndex, Player_TagPowerOn(playerIndex) != FALSE); /* needed to match: != FALSE */
    }
    return bonus + data_027c027c->unk_48[data_027c0284]->players[playerIndex].firepowerBonus;
}
