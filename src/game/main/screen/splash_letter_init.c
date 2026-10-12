#include "types.h"
#include "game/splash_task.h"

/* Places letter task n at x = (n * 4 + 8) * 8 (64, 96, 128, 160), y = 80. */
void SplashLetter_Init(SplashTask* task) {
  task->x = (task->letterIndex * 4 + 8) * 8;
  task->y = 80;
  task->height = 80;
  task->bounces = 0;
  task->restedOnce = 0;
  task->letterState = 0;
}
