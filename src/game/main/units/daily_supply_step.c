#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/script_slot.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const ScriptCommand data_021666e8[];

s8 ScriptSlot_Find(const ScriptCommand* script);
void ScriptSlot_ClearCallback(int slot);
BOOL Unit_ShowServiceCost(MapUnit* unit, int (*getCost)(MapUnit* unit));
BOOL Unit_HasAdjacentSupplier(MapUnit* unit);
int Unit_ResupplyCargo(MapUnit* transport);
int Unit_ResupplyUnpaid(MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT_SLOT \
  (data_027c027c->scriptSlots[*data_027c027c->scriptSlotCurrent])
#define UNIT_SLOTS_SCANNED 51

/* Script callback of the start-of-day resupply, run like DailyRepair_Step:
 * walks the current player's unit slots from the running slot's unk_38. A
 * Cruiser or Carrier refills the units aboard; any other unit next to a
 * supplier is refilled. Both are free. The first unit that got anything
 * starts the service cost popup and the walk stops there until the popup is
 * gone. After the last slot the callback ends. */
void DailySupply_Step(void) {
  u16 i;
  MapUnit* unit;

  if (ScriptSlot_Find(data_021666e8) != -1) {
    return;
  }
  for (i = CURRENT_SLOT.unk_38; i < UNIT_SLOTS_SCANNED; i++) {
    unit = &STATE->units[i + STATE->currentUnitBase];
    if (unit->type == 0) {
      continue;
    }
    if (unit->flags & MAP_UNIT_CARRIED) {
      continue;
    }
    if ((unit->type == UNIT_CRUISER || unit->type == UNIT_CARRIER) &&
        Unit_ShowServiceCost(unit, Unit_ResupplyCargo)) {
      CURRENT_SLOT.unk_38 = i + 1;
      break;
    }
    if (Unit_HasAdjacentSupplier(unit) &&
        Unit_ShowServiceCost(unit, Unit_ResupplyUnpaid)) {
      CURRENT_SLOT.unk_38 = i + 1;
      break;
    }
  }
  if (i != UNIT_SLOTS_SCANNED) {
    return;
  }
  ScriptSlot_ClearCallback((u8)*data_027c027c->scriptSlotCurrent);
}
