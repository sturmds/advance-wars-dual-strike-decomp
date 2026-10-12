#include "types.h"
#include "game/map_hit_task.h"
#include "game/map_object.h"
#include "game/task.h"

extern const TaskScriptCmd data_02168d68[];

BOOL MapObject_GetTargetTile(MapObject* object, s16* pos);

/* Starts the hit display for a map object under `parent`, at the tile
 * MapObject_GetTargetTile gives. */
void MapObjectHitTask_Start(MapObject* object, Task* parent) {
  s16 pos[2];
  MapHitTask* task;

  MapObject_GetTargetTile(object, pos);
  task = (MapHitTask*)Task_CreateChild(data_02168d68, parent);
  task->x = pos[0];
  task->y = pos[1];
  task->kind = object->kind;
  task->hp = object->hp;
  task->object = object;
}
