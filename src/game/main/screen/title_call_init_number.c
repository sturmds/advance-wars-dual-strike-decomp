#include "types.h"
#include "game/title_call_task.h"
#include "game/Unk_02088d04.h"

void Gfx_RequestLayerCopy(int layer);

/* First step of the title call script (data_0215bbc8): resets the frame
 * counters and scrolls layer 0 256 pixels right, off the screen, for
 * TitleCall_NumberSlideIn. */
void TitleCall_InitNumber(TitleCallTask* task) {
  task->frame = 0;
  task->hold = 0;
  Gfx_RequestLayerCopy(0);
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 256;
}
