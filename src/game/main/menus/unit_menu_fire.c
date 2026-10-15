#include "types.h"
#include "game/Unk_02080440.h"
#include "game/menu_item.h"
#include "game/task.h"
#include "game/range_map.h"

extern TaskScriptCmd gFireTargetTaskScript[];

void RangeMap_MarkUnitAttackRange(int x, int y, MapUnit* unit);
void func_0201e954(void);
void MenuDepth_Push(void);

/* Handler of the unit menu's Fire entries: marks the acting unit's attack range
 * and starts the target picker. Does nothing for the greyed-out entry. */
void UnitMenu_Fire(s8 index, s8 selected, s8 flags) {
  if (flags & MENU_ITEM_GREYED) {
    return;
  }
  RangeMap_Fill(-1);
  RangeMap_MarkUnitAttackRange(MAP_STATE->actingUnitDestX,
                               MAP_STATE->actingUnitDestY,
                               MAP_STATE->actingUnit);
  Task_Create(gFireTargetTaskScript, TASK_LIST(3));
  func_0201e954();
  if (data_027c027c->unk_50[data_027c0284]->unk_34 != NULL) {
    data_027c027c->unk_50[data_027c0284]->unk_34();
  }
  MenuDepth_Push();
}
