#include "types.h"
#include "game/map_hit_task.h"
#include "game/map_object.h"
#include "game/task.h"

void TileEffectTask_Create(int x, int y, int effect, Task* parent);
void func_020eeddc(int x, int y, Task* parent); /* tile effect 0 */
void PipeSeam_Break(int x, int y, Task* parent);
void MapObject_DestroyBlackCrystal(MapObject* object, Task* task);
void MapObject_DestroyWeakPoint(MapObject* object, Task* task);
void MapObject_DestroyMinicannon(MapObject* object, Task* task);
void func_020ed744(MapObject* object, int arg);
void func_020ed774(MapObject* object, int arg);

/* Tile effect of a hit pipe seam that still stands. */
#define TILE_EFFECT_PIPE_SEAM_HIT 28

/* Script step of the hit display: while the target has hp left it plays a
 * hit effect on the tile (effect 28 for a pipe seam, 0 otherwise);
 * otherwise it runs the destroy routine of the target's kind. */
void MapHitTask_ShowResult(MapHitTask* task) {
  if (task->hp > 0) {
    if (task->kind == MAP_HIT_PIPE_SEAM) {
      TileEffectTask_Create(task->x, task->y, TILE_EFFECT_PIPE_SEAM_HIT,
                            (Task*)task);
    } else {
      func_020eeddc(task->x, task->y, (Task*)task);
    }
    return;
  }
  switch (task->kind) {
    case MAP_HIT_PIPE_SEAM:
      PipeSeam_Break(task->x, task->y, (Task*)task);
      break;
    case MAP_OBJECT_BLACK_CRYSTAL:
      MapObject_DestroyBlackCrystal(task->object, (Task*)task);
      break;
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT + 1:
    case MAP_OBJECT_GRAND_BOLT_WEAK_POINT + 2:
      MapObject_DestroyWeakPoint(task->object, (Task*)task);
      break;
    case MAP_OBJECT_MINICANNON:
      MapObject_DestroyMinicannon(task->object, (Task*)task);
      break;
    case MAP_OBJECT_BLACK_CANNON:
      func_020ed744(task->object, (int)task);
      break;
    case MAP_OBJECT_BLACK_OBELISK:
      func_020ed774(task->object, (int)task);
      break;
  }
}
