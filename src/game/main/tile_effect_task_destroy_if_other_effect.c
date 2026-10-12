#include "types.h"
#include "game/task.h"
#include "game/tile_effect_task.h"

extern int gTileEffectStarting;

void Task_Destroy(Task* task);

/* Visitor for TileEffectTask_Create: destroys a tile effect task unless it
 * plays the effect being started. */
void TileEffectTask_DestroyIfOtherEffect(TileEffectTask* task) {
  if (task->effect != gTileEffectStarting) {
    Task_Destroy((Task*)task);
  }
}
