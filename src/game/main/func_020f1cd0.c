#include "types.h"
#include "game/task.h"
#include "game/screen_flash.h"
#include "game/screen.h"

void func_020f1cd0(Task* task) {
  ScreenShake_Start(2, 160, task);
  ScreenFlash_Start(160, 30, 50, task);
}
