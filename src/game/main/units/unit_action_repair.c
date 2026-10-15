#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"
#include "game/repair_task.h"

extern s16 data_021684d8[]; /* dx, dy per direction */
extern const TaskScriptCmd data_02168538[];

/* Unit action 24: the acting unit repairs the unit on the adjacent tile in
 * the given direction (0-3). */
void UnitAction_Repair(int direction) {
  int i;
  MapUnit* unit;
  int x;
  int y;
  RepairTask* task;
  i = direction * 2;

  x = MAP_STATE->actingUnitDestX + data_021684d8[i];
  y = MAP_STATE->actingUnitDestY + data_021684d8[i + 1];
  unit = &MAP_STATE->units[MAP_STATE->mapTiles
                               ->unk_508[x + MAP_STATE->mapTiles->rowStart[y]]];
  if (unit == NULL) {
    return;
  }
  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.suppliesAndRepairs < 999999) {
      gPlayRecords.suppliesAndRepairs++;
    }
  }
  task = (RepairTask*)Task_Create(data_02168538, TASK_LIST(3));
  task->tileX = x;
  task->tileY = y;
  task->unit = unit;
}
