#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSpriteTask.h"
#include "game/task.h"

/* Destroys the sprite tasks of a cell task's entry and clears their pointers. */
void BattleCellTask_DestroySprites(BattleCellTask* task) {
  if (CELL_TASK_ENTRY(task).sprites[0] != NULL) {
    Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[0]);
    CELL_TASK_ENTRY(task).sprites[0] = NULL;
  }
  if (CELL_TASK_ENTRY(task).sprites[1] != NULL) {
    Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[1]);
    CELL_TASK_ENTRY(task).sprites[1] = NULL;
  }
  if (CELL_TASK_ENTRY(task).sprites[2] != NULL) {
    Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[2]);
    CELL_TASK_ENTRY(task).sprites[2] = NULL;
  }
  if (CELL_TASK_ENTRY(task).sprites[3] != NULL) {
    Task_Destroy((Task*)CELL_TASK_ENTRY(task).sprites[3]);
    CELL_TASK_ENTRY(task).sprites[3] = NULL;
  }
}
