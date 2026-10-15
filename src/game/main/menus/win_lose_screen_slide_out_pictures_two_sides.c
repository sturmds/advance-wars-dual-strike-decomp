#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/win_lose.h"
#include "game/math.h"
#include "game/screen_other_map.h"

void Task_ResumeScript(WinLoseTask* task);
void Co_QueuePictureSprite(int x, int y, int attr, int list);

#define MAIN_MAP (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

#define SWITCH_TO(state)                     \
  {                                          \
    Unk_027c0280* saved0280 = data_027c0280; \
    Unk_02080440* saved027c;                 \
    Screen_SetCurrentGfx(state);             \
    saved027c = data_027c027c;               \
    Screen_SetCurrent(state)

#define SWITCH_BACK()              \
  Screen_SetCurrent(saved027c);    \
  Screen_SetCurrentGfx(saved0280); \
  }

/* OBJ attribute 2 of CO picture `slot`: palette `slot`, priority 2, 36
 * tiles per picture. */
#define PORTRAIT_ATTR(slot) (((slot) << 12) | 0x800 | ((slot) * 36))

/* One frame of the two-side win/lose screen: side A's CO pictures (second
 * screen, y 192) slide off to the left, side B's (main screen, y 48,
 * flipped) to the right. Picture i starts at frame 6 * i and takes 18
 * frames. When the last one is gone the frame counter is set to 64 and
 * the task's script resumes. */
void WinLoseScreen_SlideOutPicturesTwoSides(WinLoseTask* task) {
  int i;
  int x;

  SWITCH_TO(SECOND_MAP);
  for (i = 0; i < task->portraitsA; i++) {
    if (task->frame < i * 6) {
      Co_QueuePictureSprite((24 + i * 32) & 0x1ff, 192, PORTRAIT_ATTR(i), i);
    } else if (task->frame < i * 6 + 18) {
      x = Math_Ease(1, 0, -240, task->frame - i * 6, 18);
      x += 24 + i * 32;
      if (x > -48) {
        Co_QueuePictureSprite(x & 0x1ff, 192, PORTRAIT_ATTR(i), i);
      }
    }
  }
  SWITCH_BACK();

  SWITCH_TO(MAIN_MAP);
  for (i = 0; i < task->portraitsB; i++) {
    if (task->frame < i * 6) {
      Co_QueuePictureSprite(((232 - i * 32) & 0x1ff) | OAM_ATTR1_HFLIP, 48,
                            PORTRAIT_ATTR(i), i);
    } else if (task->frame < i * 6 + 18) {
      x = (232 - i * 32) - Math_Ease(1, 0, -256, task->frame - i * 6, 18);
      if (x < 256) {
        Co_QueuePictureSprite((x & 0x1ff) | OAM_ATTR1_HFLIP, 48,
                              PORTRAIT_ATTR(i), i);
      }
    }
  }
  SWITCH_BACK();

  if (task->frame != (task->portraitsMax - 1) * 6 + 18) {
    task->frame++;
    return;
  }
  task->frame = 64;
  Task_ResumeScript(task);
}
