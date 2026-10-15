#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/script_slot.h"
#include "game/unit_stats.h"

extern const ScriptCommand data_021666e8[];

s8 ScriptSlot_Find(const ScriptCommand* script);
void ScriptSlot_ClearCallback(int slot);
void ServiceCostPopup_SetShown(void);
void ServiceCostPopup_Start(s16* pos, int player, int cost, u32 arg3, int arg4,
                            int arg5);
int Unit_Rearm(MapUnit* unit, BOOL payCost);
int Unit_Refuel(MapUnit* unit, BOOL payCost);
int Unit_Repair(MapUnit* unit, s16 steps, BOOL payCost);
int Player_GetDailyRepairHp(
    int player); /* shown HP a day of repairs gives the player's units (calls Player_GetRepairHp) */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT_SLOT \
  (data_027c027c->scriptSlots[*data_027c027c->scriptSlotCurrent])
#define UNIT_SLOTS_SCANNED 51

/* Script callback of the start-of-day repairs. Walks the current player's
 * unit slots from the one kept in the running slot's unk_38. A unit on a
 * property of its owner that repairs its type gets ammo and fuel for free and
 * the player's daily repair, paid. The first unit that got anything starts the
 * service cost popup and the walk stops there until the popup is gone. With a
 * repair the funds are put back as they were before it and the popup gets the
 * repair cost alone; arg3 tells it a repair was made. After the last slot the
 * callback ends. */
void DailyRepair_Step(void) {
  s16 pos[2];
  u16 i;
  u8 repaired;
  int cost;
  int repairCost;
  MapUnit* unit;
  int funds;

  if (ScriptSlot_Find(data_021666e8) != -1) {
    return;
  }
  for (i = CURRENT_SLOT.unk_38; i < UNIT_SLOTS_SCANNED; i++) {
    unit = &MAP_STATE->units[i + MAP_STATE->currentUnitBase];
    if (unit->type == 0) {
      continue;
    }
    if (unit->flags & MAP_UNIT_CARRIED) {
      continue;
    }
    if (MAP_STATE->unk_04 != (MAP_TILE_AT(unit->x, unit->y) & 0xe0)) {
      continue;
    }
    if (Ov000_gUnitStats[unit->type]
            .repairTerrain[MAP_TILE_TERRAIN(MAP_TILE_AT(unit->x, unit->y))] ==
        0) {
      continue;
    }
    funds = MAP_STATE->players[MAP_STATE->currentPlayer].funds;
    cost = 0;
    cost += Unit_Rearm(unit, FALSE);
    cost += Unit_Refuel(unit, FALSE);
    repairCost = Unit_Repair(
        unit, Player_GetDailyRepairHp(MAP_STATE->currentPlayer), TRUE);
    if (cost == 0 && repairCost == 0) {
      continue;
    }
    if (repairCost != 0) {
      MAP_STATE->players[MAP_STATE->currentPlayer].funds = funds;
      repaired = TRUE;
      cost = 0;
    } else {
      repaired = FALSE;
    }
    pos[0] = unit->x;
    pos[1] = unit->y;
    ServiceCostPopup_Start(
        pos, (u8)MAP_STATE->currentPlayer, repairCost + cost, repaired, 0,
        MAP_STATE->mapTiles->unk_2338[MAP_TILE_INDEX(unit->x, unit->y)]);
    CURRENT_SLOT.unk_38 = i + 1;
    break;
  }
  if (i != UNIT_SLOTS_SCANNED) {
    return;
  }
  ServiceCostPopup_SetShown();
  ScriptSlot_ClearCallback((u8)*data_027c027c->scriptSlotCurrent);
}
