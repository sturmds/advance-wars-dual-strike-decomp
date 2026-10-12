#include "types.h"
#include "game/BattleCellTask.h"
#include "game/BattleSpriteTask.h"

/* Copies the task's two positions to one sprite of its cell, when that sprite exists. */
#define COPY_POS_TO_SPRITE(task, i)                             \
  do {                                                          \
    if (CELL_TASK_ENTRY(task).sprites[i] != NULL) {             \
      CELL_TASK_ENTRY(task).sprites[i]->unk_3c = (task)->posX;  \
      CELL_TASK_ENTRY(task).sprites[i]->unk_3e = (task)->posY;  \
      CELL_TASK_ENTRY(task).sprites[i]->unk_42 = (task)->pos2X; \
      CELL_TASK_ENTRY(task).sprites[i]->unk_44 = (task)->pos2Y; \
    }                                                           \
  } while (0)

/* While moving is set, moves the cell: adds the acceleration to the velocity, the velocity
 * (8.8 fixed point) to both positions, and copies the positions to the cell's sprites. */
void BattleCellTask_Move(BattleCellTask* task) {
  if (!task->moving) {
    return;
  }
  task->velX += task->accelX;
  task->velY += task->accelY;
  task->posX += task->velX >> 8;
  task->posY += task->velY >> 8;
  task->pos2X += task->velX >> 8;
  task->pos2Y += task->velY >> 8;
  COPY_POS_TO_SPRITE(task, 0);
  COPY_POS_TO_SPRITE(task, 1);
  COPY_POS_TO_SPRITE(task, 2);
  COPY_POS_TO_SPRITE(task, 3);
}
