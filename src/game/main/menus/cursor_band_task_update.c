#include "types.h"
#include "game/cursor_band_task.h"
#include "game/menu_button.h"
#include "game/Unk_02088d04.h"

void Sprite_QueueAt(int list, int x, int y, void* data, int attr, int tile);

extern u8 data_027c0048[];

/* Draws nine 32-pixel sprites across the screen at the menu cursor's
 * height, scrolled by scrollX, in one of four frames. The frames go 0, 1,
 * 2, 3, 2, 1 and hold for 18, 3, 2, 1, 2 and 3 frames. */
void CursorBandTask_Update(CursorBandTask* task) {
  s16 y = MenuCursor_GetCenterY();
  int frame;
  int i;

  switch (task->step) {
    case 0:
      frame = 0;
      if (task->timer == 18) {
        task->step = 1;
        task->timer = 0;
      }
      break;
    case 1:
    case 5:
      frame = 1;
      if (task->timer == 3) {
        if (task->step == 1) {
          task->step = 2;
        } else {
          task->step = 0;
        }
        task->timer = 0;
      }
      break;
    case 2:
    case 4:
      frame = 2;
      if (task->timer == 2) {
        if (task->step == 2) {
          task->step = 3;
        } else {
          task->step = 5;
        }
        task->timer = 0;
      }
      break;
    case 3:
      frame = 3;
      if (task->timer == 1) {
        task->step = 4;
        task->timer = 0;
      }
      break;
  }
  for (i = -1; i < SCREEN_WIDTH / 32; i++) {
    Sprite_QueueAt(4, (task->scrollX + i * 32) & 0x1ff, (y - 16) & 0xff,
                   &data_027c0048, (frame * 16 + 10) | 0x1800, 0);
  }
  task->timer++;
  task->scrollX += 2;
  task->scrollX &= 31;
}
