#include "types.h"
#include "game/cannon_shot_task.h"
#include "game/task.h"

extern TaskScriptCmd data_02168088[];

void MapScript_RunList10(int id, int unit);
void func_020f5878(s16 x, s16 y);

/* Starts the cannon shot script under parent, then scrolls the map to the
 * tile one down and one right of the cannon. */
void CannonShotTask_Start(int cannonX, int cannonY, int targetX, int targetY,
                          int kind, int facing, Task* parent) {
  CannonShotTask* task;

  MapScript_RunList10(44, 0);
  task = (CannonShotTask*)Task_CreateChild(data_02168088, parent);
  task->cannonX = cannonX;
  task->cannonY = cannonY;
  task->targetX = targetX;
  task->targetY = targetY;
  task->facing = facing;
  task->cannonKind = kind;
  task->unk_5c = 0;
  func_020f5878(cannonX + 1, cannonY + 1);
}
