#include "types.h"
#include "game/wallpaper_task.h"
#include "game/Unk_02088d04.h"

/* Task step that sets up the wallpaper task: clears its counters and sets an
 * alpha blend of layer 2 over layer 3 that shows layer 2 alone. */
void WallpaperTask_Init(WallpaperTask* task) {
  task->scroll2 = 32;
  task->scroll3 = 0;
  task->blendStep = 0;
  task->frame = 0;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendTopBackdrop = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 0;
  data_027c0280->blendBottomBackdrop = 0;
}
