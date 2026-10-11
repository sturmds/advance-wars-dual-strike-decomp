#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

void func_020c0704(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Clears the funds, statistics, COs and power state of all five player
 * slots and puts each slot on the team of its own number. */
void Players_Reset(void)
{
    int slot;
    int unitType;

    for (slot = 0; slot < 5; slot++) {
        STATE->players[slot].funds = 0;
        STATE->players[slot].unk_04 = 0;
        STATE->players[slot].damageTaken = 0;
        STATE->players[slot].propertyCount = 0;
        STATE->players[slot].defeatCode = 0;
        STATE->players[slot].defeated = 0;
        STATE->players[slot].turnKills = 0;
        STATE->players[slot].mostTurnKills = 0;
        STATE->players[slot].destroyedCounts[0] = 0;
        STATE->players[slot].destroyedCounts[1] = 0;
        STATE->players[slot].destroyedCounts[2] = 0;
        STATE->players[slot].destroyedCounts[3] = 0;
        STATE->players[slot].destroyedCounts[4] = 0;
        STATE->players[slot].armyColour = 0;
        STATE->players[slot].controller = PLAYER_CONTROLLER_NONE;
        STATE->players[slot].visionGrids = 0;
        STATE->players[slot].front.id = 0;
        STATE->players[slot].back.id = 0;
        STATE->players[slot].front.powerState = 0;
        STATE->players[slot].front.unk_12 = 0;
        STATE->players[slot].front.charge = 0;
        STATE->players[slot].back.charge = 0;
        STATE->players[slot].front.powerReady = 0;
        STATE->players[slot].front.powerUses = 0;
        STATE->players[slot].back.powerUses = 0;
        STATE->players[slot].tagPowerHalf = 0;
        STATE->players[slot].firepowerBonus = 0;
        STATE->players[slot].defenseBonus = 0;
        STATE->players[slot].team = slot;
        STATE->players[slot].unk_33 = 0;
        STATE->players[slot].unk_34 = 0;
        STATE->players[slot].unitCount = 0;
        for (unitType = 0; unitType < UNIT_TYPE_COUNT; unitType++) {
            STATE->players[slot].unitsBuilt[unitType] = 0;
            STATE->players[slot].unitsLost[unitType] = 0;
        }
        STATE->players[slot].playTimeFrames = 0;
    }
    func_020c0704();
}
