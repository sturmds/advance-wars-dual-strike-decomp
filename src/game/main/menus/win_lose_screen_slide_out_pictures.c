#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
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

/* OBJ attribute 1: horizontal flip, for the pictures on the right. */
#define OBJ_HFLIP 0x1000

/* OBJ attribute 2 of CO picture `slot` (see WinLoseScreen_Init): palette
 * number `slot`, priority 2, 36 tiles per picture. */
#define PORTRAIT_ATTR(slot) (((slot) << 12) | 0x800 | ((slot) * 36))

/* One frame of the one-side win/lose screen: the CO pictures loaded by
 * WinLoseScreen_Init slide off the screen edges, the left pictures to the
 * left and the right ones (flipped) to the right. Players 0-1 are on the
 * second screen, 2-3 on the main one. Tag partners (bits of portraitsA)
 * start 6 frames later. At frame 24, or at 18 when a tag partner is
 * shown, the frame counter is set to 64 and the task's script resumes. */
void WinLoseScreen_SlideOutPictures(WinLoseTask* task) {
  int i;
  int slideFront;
  int slideBack;

  task->frame++;
  if (task->frame >= 18) {
    slideFront = 256;
  } else {
    slideFront = Math_Ease(1, 0, 256, task->frame, 18);
  }
  if (task->frame < 6) {
    slideBack = 0;
  } else {
    slideBack = Math_Ease(1, 0, 256, task->frame - 6, 18);
  }

  for (i = 0; i < data_022a7a88.countA; i++) {
    if (i < 2) {
      SWITCH_TO(SECOND_MAP);
      if (i == 0) {
        /* Needed to match: `i << 13` rather than PORTRAIT_ATTR(i * 2). */
        if (24 - slideFront > -24) {
          Co_QueuePictureSprite((24 - slideFront) & 0x1ff, 192,
                                (i << 13) | 0x800 | (i * 2 * 36), 0);
        }
        if ((task->portraitsA & (1 << i)) && 24 - slideBack > -24) {
          Co_QueuePictureSprite((56 - slideBack) & 0x1ff, 192,
                                PORTRAIT_ATTR(i * 2 + 1), 1);
        }
      } else {
        if (slideFront + 232 < 280) {
          Co_QueuePictureSprite(((slideFront + 232) & 0x1ff) | OBJ_HFLIP, 192,
                                (i << 13) | 0x800 | (i * 2 * 36), 0);
        }
        if ((task->portraitsA & (1 << i)) && slideBack + 232 < 280) {
          Co_QueuePictureSprite(((slideBack + 200) & 0x1ff) | OBJ_HFLIP, 192,
                                PORTRAIT_ATTR(i * 2 + 1), 1);
        }
      }
      SWITCH_BACK();
    } else {
      SWITCH_TO(MAIN_MAP);
      if (i + 1 == 3) { /* player 2; this form needed to match */
        if (24 - slideFront > -24) {
          Co_QueuePictureSprite((24 - slideFront) & 0x1ff, 48,
                                PORTRAIT_ATTR((i - 2) * 2), 0);
        }
        if ((task->portraitsA & (1 << i)) && 24 - slideBack > -24) {
          Co_QueuePictureSprite((56 - slideBack) & 0x1ff, 48,
                                PORTRAIT_ATTR((i - 2) * 2 + 1), 1);
        }
      } else {
        if (slideFront + 232 < 280) {
          Co_QueuePictureSprite(((slideFront + 232) & 0x1ff) | OBJ_HFLIP, 48,
                                PORTRAIT_ATTR((i - 2) * 2), 0);
        }
        if ((task->portraitsA & (1 << i)) && slideBack + 232 < 280) {
          Co_QueuePictureSprite(((slideBack + 200) & 0x1ff) | OBJ_HFLIP, 48,
                                PORTRAIT_ATTR((i - 2) * 2 + 1), 1);
        }
      }
      SWITCH_BACK();
    }
  }

  if (task->frame == 24 || (task->portraitsA != 0 && task->frame == 18)) {
    task->frame = 64;
    Task_ResumeScript(task);
  }
}
