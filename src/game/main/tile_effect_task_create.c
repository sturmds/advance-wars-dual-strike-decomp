#include "types.h"
#include "game/task.h"
#include "game/tile_effect_task.h"

extern const TaskScriptCmd gTileEffectTaskScript[];
extern int gTileEffectStarting;

void TileEffectTask_DestroyIfOtherEffect(TileEffectTask* task);
void Task_ForEachByScript(const TaskScriptCmd* script,
                          void (*visit)(TileEffectTask* task));

/* Starts a tile effect on tile (x, y). Tile effect tasks that play another
 * effect are destroyed first. */
void TileEffectTask_Create(int x, int y, int effect, Task* parent) {
  TileEffectTask* task;

  gTileEffectStarting = effect;
  Task_ForEachByScript(gTileEffectTaskScript,
                       TileEffectTask_DestroyIfOtherEffect);
  task = (TileEffectTask*)Task_CreateChild(gTileEffectTaskScript, parent);
  task->sprite = NULL;
  task->x = x;
  task->y = y;
  task->effect = effect;
}
