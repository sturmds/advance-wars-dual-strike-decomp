#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/tile_strike_task.h"

extern const TaskScriptCmd data_02167ef0[];
extern const TaskScriptCmd data_02167f20[];

/* Starts a tile strike task under parent (a task list level or a task).
 * The task for the screen that shows the 3D engine draws in 3D. */
void TileStrike_Start(int tileX, int tileY, void* parent) {
  TileStrikeTask* task;

  if (data_027c027c == data_027c0288.unk_00) {
    task = (TileStrikeTask*)Task_Create(data_02167f20, parent);
    task->tileX = tileX;
    task->tileY = tileY;
  } else {
    task = (TileStrikeTask*)Task_Create(data_02167ef0, parent);
    task->tileX = tileX;
    task->tileY = tileY;
  }
}
