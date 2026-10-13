#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/oam_cells.h"
#include "game/win_lose.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Task_ResumeScript(WinLoseTask* task);
void Co_QueuePictureSprite(int x, int y, int attr, int list);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

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

/* One frame of the two-side win/lose screen: the CO pictures slide in,
 * side A's (second screen, y 192) from the right, side B's (main screen,
 * y 48, flipped) from the left. Picture i starts at frame 6 * i and takes
 * 18 frames. When the last one is in place the task's script resumes. */
void WinLoseScreen_SlideInPicturesTwoSides(WinLoseTask* task) {
  int i;
  int slide;

  task->frame++;
  SWITCH_TO(SECOND_MAP);
  for (i = 0; i < task->portraitsA; i++) {
    if (task->frame >= i * 6) {
      if (task->frame < i * 6 + 18) {
        slide = Math_Ease(4, 256, 0, task->frame - i * 6, 18);
        Co_QueuePictureSprite((slide + (24 + i * 32)) & 0x1ff, 192,
                              PORTRAIT_ATTR(i), i);
      } else {
        Co_QueuePictureSprite((24 + i * 32) & 0x1ff, 192, PORTRAIT_ATTR(i), i);
      }
    }
  }
  SWITCH_BACK();

  SWITCH_TO(MAIN_MAP);
  for (i = 0; i < task->portraitsB; i++) {
    if (task->frame >= i * 6) {
      if (task->frame < i * 6 + 18) {
        slide = Math_Ease(4, 256, 0, task->frame - i * 6, 18);
        Co_QueuePictureSprite(
            (((232 - i * 32) - slide) & 0x1ff) | OAM_ATTR1_HFLIP, 48,
            PORTRAIT_ATTR(i), i);
      } else {
        Co_QueuePictureSprite(((232 - i * 32) & 0x1ff) | OAM_ATTR1_HFLIP, 48,
                              PORTRAIT_ATTR(i), i);
      }
    }
  }
  SWITCH_BACK();

  if (task->frame == (task->portraitsMax - 1) * 6 + 18) {
    task->frame = 0;
    Task_ResumeScript(task);
  }
}
