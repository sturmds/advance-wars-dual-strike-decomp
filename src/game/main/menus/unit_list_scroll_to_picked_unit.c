#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_list_task.h"

void func_020f5888(int x, int y);

/* Script step of the unit list menu: when a row was picked, scrolls the map to
 * that unit (pickedSlot counts from the current player's first slot). */
void UnitList_ScrollToPickedUnit(UnitListTask* task) {
  MapUnit* unit;

  if (task->pickedSlot == 0) {
    return;
  }
  unit = &MAP_STATE->units[task->pickedSlot + MAP_STATE->currentUnitBase];
  func_020f5888(unit->x, unit->y);
}
