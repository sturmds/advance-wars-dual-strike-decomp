#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_hit_task.h"
#include "game/task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const TaskScriptCmd data_02168d68[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Starts the hit display for the pipe seam at tile (x, y) under `parent`;
 * it shows the hp the tile has left. */
void PipeSeamHitTask_Start(int x, int y, Task* parent) {
  MapHitTask* task = (MapHitTask*)Task_CreateChild(data_02168d68, parent);

  task->x = x;
  task->y = y;
  task->kind = MAP_HIT_PIPE_SEAM;
  task->hp = STATE->tileHp[x + STATE->mapTiles->rowStart[y]] & 0x7f;
  task->object = NULL;
}
