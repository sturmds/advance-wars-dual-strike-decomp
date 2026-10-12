#include "types.h"
#include "game/task.h"
#include "game/screen_flash.h"

void ScreenShake_Start(int script, int frames, Task* parent);

void func_020f1cd0(Task* task) {
  ScreenShake_Start(2, 160, task);
  ScreenFlash_Start(160, 30, 50, task);
}
