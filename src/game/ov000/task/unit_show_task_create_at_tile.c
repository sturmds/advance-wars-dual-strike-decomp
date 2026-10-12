#include "types.h"
#include "game/Unk_02080440.h"
#include "game/task.h"
#include "game/unit_show_task.h"

extern Unk_02080440* data_027c027c;
extern TaskScriptCmd data_ov000_022db6e4[];

/* Starts a show task for a unit type at a tile, with no unit behind it
 * (task->unit is NULL). Does nothing while both show slots are busy. */
void UnitShowTask_CreateAtTile(int tileX, int tileY, int unitType) {
  int slot;
  UnitShowTask* task;

  for (slot = 0; slot < UNIT_SHOW_SLOT_COUNT; slot++) {
    if (data_027c027c->unk_58[slot] == 0) {
      break;
    }
  }
  if (slot == UNIT_SHOW_SLOT_COUNT) {
    return;
  }
  task = (UnitShowTask*)Task_Create(data_ov000_022db6e4, TASK_LIST(5));
  task->x = tileX << 4;
  task->y = tileY << 4;
  task->state = UNIT_SHOW_STATE_MOVE;
  task->entry = unitType;
  task->unit = NULL;
}
