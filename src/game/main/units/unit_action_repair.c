#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"
#include "game/repair_task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 data_021684d8[]; /* dx, dy per direction */
extern const TaskScriptCmd data_02168538[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Unit action 24: the acting unit repairs the unit on the adjacent tile in
 * the given direction (0-3). */
void UnitAction_Repair(int direction) {
  int i;
  MapUnit* unit;
  int x;
  int y;
  RepairTask* task;
  i = direction * 2;

  x = GAME->actingUnitDestX + data_021684d8[i];
  y = GAME->actingUnitDestY + data_021684d8[i + 1];
  unit = &GAME->units[GAME->mapTiles->unk_508[x + GAME->mapTiles->rowStart[y]]];
  if (unit == NULL) {
    return;
  }
  if (GAME->players[GAME->currentPlayer].controller ==
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
