#include "types.h"
#include "game/task.h"
#include "game/screen_flash.h"
#include "game/screen_layer0_blend.h"
#include "game/ex_machina_bolt_task.h"
#include "game/screen.h"

/* Script step of gExMachinaBoltScript: flashes the screen white and shakes it for
 * 90 frames as the bolt hits, and sets layer 0 (the bolt graphics) up to
 * blend in; the next step fades it in on task->frame. */
void ExMachinaBolt_StartFlash(ExMachinaBoltTask* task) {
  ScreenFlash_Start(4, 0, 20, NULL);
  ScreenShake_Start(2, 90, NULL);
  Screen_SetLayer0Blend();
  task->frame = 0;
}
