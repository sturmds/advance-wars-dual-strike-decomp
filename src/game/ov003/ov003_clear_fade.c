#include "types.h"
#include "game/screen_other_map.h"
#include "game/ov003_frame_task.h"

void Screen_SetCurrentBoth(void* state);

/* Restarts the task's frame count and sets the master brightness of both
 * screens to 0 (no fade). */
void Ov003_ClearFade(Ov003FrameTask* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  task->frame = 0;
  savedGfx = data_027c0280;
  savedScreen = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness = 0;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness = 0;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);
}
