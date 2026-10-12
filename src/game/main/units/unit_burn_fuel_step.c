#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const TaskScriptCmd data_02168f48[];

void* Task_FindByScript(const void* script);
BOOL Unit_BurnDailyFuel(MapUnit* unit);
void func_020edb84(MapUnit* unit);
void Task_ResumeScript(Task* task);

#define GAME (data_027c027c->unk_48[data_027c0284])
#define UNIT_SLOTS_SCANNED 51

/* Step of a start-of-day task script. Walks the current player's unit slots
 * from the slot kept in unk_64, burning each unit's daily fuel. When a unit
 * runs out, func_020edb84 starts its crash task and this step returns; it
 * continues from the next slot once that task is gone. The script resumes
 * after the last slot. */
void DailyFuel_BurnStep(Task* task) {
  if (Task_FindByScript(data_02168f48) != NULL) {
    return;
  }
  /* needed to match: unk_64 is read as signed */
  while (task->unk_64 < UNIT_SLOTS_SCANNED) {
    MapUnit* unit = &GAME->units[task->unk_64 + GAME->currentUnitBase];

    if (unit->type != 0 && Unit_BurnDailyFuel(unit)) {
      func_020edb84(unit);
      return;
    }
    (task->unk_64)++;
  }
  if (task->unk_64 < UNIT_SLOTS_SCANNED) {
    return;
  }
  Task_ResumeScript(task);
}
