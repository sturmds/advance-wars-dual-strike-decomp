#include "types.h"
#include "game/wallpaper_task.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Task update of the wallpaper: every 8 frames scrolls layer 2 one pixel
 * left and layer 3 one pixel right (both wrap at 64). Every 4 frames takes
 * one step of a 128-step blend cycle: layer 2 alone, a 16-step fade to
 * layer 3, layer 3 alone, a 16-step fade back. */
void WallpaperTask_Update(WallpaperTask* task) {
  if ((task->frame & 7) == 0) {
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = task->scroll2 & 63;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] =
        64 - (task->scroll3 & 63);
    task->scroll2++;
    task->scroll3++;
  }
  if ((task->frame & 3) == 0) {
    switch (task->blendStep & 0x70) {
      case 0x00:
      case 0x10:
      case 0x20:
        data_027c0280->blendMode = BLEND_MODE_ALPHA;
        data_027c0280->blendEva = 16;
        data_027c0280->blendEvb = 0;
        data_027c0280->blendEvy = 0;
        break;
      case 0x30:
        data_027c0280->blendMode = BLEND_MODE_ALPHA;
        data_027c0280->blendEva = 16 - (task->blendStep & 15);
        data_027c0280->blendEvb = task->blendStep & 15;
        data_027c0280->blendEvy = 0;
        break;
      case 0x40:
      case 0x50:
      case 0x60:
        data_027c0280->blendMode = BLEND_MODE_ALPHA;
        data_027c0280->blendEva = 0;
        data_027c0280->blendEvb = 16;
        data_027c0280->blendEvy = 0;
        break;
      case 0x70:
        data_027c0280->blendMode = BLEND_MODE_ALPHA;
        data_027c0280->blendEva = task->blendStep & 15;
        data_027c0280->blendEvb = 16 - (task->blendStep & 15);
        data_027c0280->blendEvy = 0;
        break;
    }
    task->blendStep++;
  }
  task->frame++;
}
