#include "types.h"
#include "game/unit_show_task.h"

/* One (dx, dy) pair of tile steps per sprite frame (the frame gives the direction). */
extern s16 data_ov000_022db6a4[];

#define DIR_DX(task) (data_ov000_022db6a4[(task)->frame * 2])
#define DIR_DY(task) (data_ov000_022db6a4[(task)->frame * 2 + 1])

/* Moves the task by its step size, in sixteenths of a tile, along the direction
 * of its frame. When the move passes one whole tile (16 sixteenths), it takes
 * back the overshoot and resets the counter (waitFrames counts the sixteenths). */
void UnitShowTask_StateMove(UnitShowTask* task) {
  task->waitFrames += task->stepSize;
  task->x += task->stepSize * DIR_DX(task);
  task->y += task->stepSize * DIR_DY(task);
  if (task->waitFrames >= 16) {
    task->waitFrames -= 16;
    task->x -= task->waitFrames * DIR_DX(task);
    task->y -= task->waitFrames * DIR_DY(task);
    task->waitFrames = 0;
  }
}
