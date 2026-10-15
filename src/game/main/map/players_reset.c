#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

void func_020c0704(void);

/* Clears the funds, statistics, COs and power state of all five player
 * slots and puts each slot on the team of its own number. */
void Players_Reset(void) {
  int slot;
  int unitType;

  for (slot = 0; slot < ARMY_SLOT_COUNT; slot++) {
    MAP_STATE->players[slot].funds = 0;
    MAP_STATE->players[slot].unk_04 = 0;
    MAP_STATE->players[slot].damageTaken = 0;
    MAP_STATE->players[slot].propertyCount = 0;
    MAP_STATE->players[slot].defeatCode = 0;
    MAP_STATE->players[slot].defeated = 0;
    MAP_STATE->players[slot].turnKills = 0;
    MAP_STATE->players[slot].mostTurnKills = 0;
    MAP_STATE->players[slot].destroyedCounts[0] = 0;
    MAP_STATE->players[slot].destroyedCounts[1] = 0;
    MAP_STATE->players[slot].destroyedCounts[2] = 0;
    MAP_STATE->players[slot].destroyedCounts[3] = 0;
    MAP_STATE->players[slot].destroyedCounts[4] = 0;
    MAP_STATE->players[slot].armyColour = 0;
    MAP_STATE->players[slot].controller = PLAYER_CONTROLLER_NONE;
    MAP_STATE->players[slot].visionGrids = 0;
    MAP_STATE->players[slot].front.id = 0;
    MAP_STATE->players[slot].back.id = 0;
    MAP_STATE->players[slot].front.powerState = 0;
    MAP_STATE->players[slot].front.unk_12 = 0;
    MAP_STATE->players[slot].front.charge = 0;
    MAP_STATE->players[slot].back.charge = 0;
    MAP_STATE->players[slot].front.powerReady = 0;
    MAP_STATE->players[slot].front.powerUses = 0;
    MAP_STATE->players[slot].back.powerUses = 0;
    MAP_STATE->players[slot].tagPowerHalf = 0;
    MAP_STATE->players[slot].firepowerBonus = 0;
    MAP_STATE->players[slot].defenseBonus = 0;
    MAP_STATE->players[slot].team = slot;
    MAP_STATE->players[slot].unk_33 = 0;
    MAP_STATE->players[slot].unk_34 = 0;
    MAP_STATE->players[slot].unitCount = 0;
    for (unitType = 0; unitType < UNIT_TYPE_COUNT; unitType++) {
      MAP_STATE->players[slot].unitsBuilt[unitType] = 0;
      MAP_STATE->players[slot].unitsLost[unitType] = 0;
    }
    MAP_STATE->players[slot].playTimeFrames = 0;
  }
  func_020c0704();
}
