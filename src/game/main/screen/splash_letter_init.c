#include "types.h"
#include "game/splash_task.h"

/* Places letter task n at x = (n * 4 + 8) * 8 (64, 96, 128, 160), y = 80. */
void SplashLetter_Init(SplashTask *task)
{
    task->x = (task->letterIndex * 4 + 8) * 8;
    task->y = 80;
    task->unk_33 = 80;
    task->unk_35 = 0;
    task->unk_37 = 0;
    task->unk_30 = 0;
}
