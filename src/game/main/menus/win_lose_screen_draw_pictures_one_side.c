#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/win_lose.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
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

/* OBJ attribute 2 of CO picture `slot`: palette `slot`, priority 2, 36
 * tiles per picture. */
#define PORTRAIT_ATTR(slot) (((slot) << 12) | 0x800 | ((slot) * 36))

/* PORTRAIT_ATTR(pair * 2), the front CO of a picture pair. Needed to match:
 * written with `pair << 13`. */
#define FRONT_ATTR(pair) (((pair) << 13) | 0x800 | ((pair) * 2 * 36))

/* Draws the CO pictures of the one-side win/lose screen at rest, where
 * WinLoseScreen_SlideOutPictures starts them: players 0-1 on the second
 * screen (y 192), 2-3 on the main one (y 48), the first of each screen on
 * the left and the second flipped on the right. Tag partners (bits of
 * portraitsA) sit 32 pixels further in. */
void WinLoseScreen_DrawPicturesOneSide(WinLoseTask* task) {
  int i;

  for (i = 0; i < data_022a7a88.countA; i++) {
    if (i < 2) {
      SWITCH_TO(SECOND_MAP);
      if (i == 0) {
        Co_QueuePictureSprite(24, 192, FRONT_ATTR(i), 0);
        if (task->portraitsA & (1 << i)) {
          Co_QueuePictureSprite(56, 192, PORTRAIT_ATTR(i * 2 + 1), 1);
        }
      } else {
        Co_QueuePictureSprite(232 | OBJ_HFLIP, 192, FRONT_ATTR(i), 0);
        if (task->portraitsA & (1 << i)) {
          Co_QueuePictureSprite(200 | OBJ_HFLIP, 192, PORTRAIT_ATTR(i * 2 + 1),
                                1);
        }
      }
      SWITCH_BACK();
    } else {
      SWITCH_TO(MAIN_MAP);
      if (i + 1 == 3) { /* player 2; this form needed to match */
        Co_QueuePictureSprite(24, 48, FRONT_ATTR(i - 2), 0);
        if (task->portraitsA & (1 << i)) {
          Co_QueuePictureSprite(56, 48, PORTRAIT_ATTR((i - 2) * 2 + 1), 1);
        }
      } else {
        Co_QueuePictureSprite(232 | OBJ_HFLIP, 48, FRONT_ATTR(i - 2), 0);
        if (task->portraitsA & (1 << i)) {
          Co_QueuePictureSprite(200 | OBJ_HFLIP, 48,
                                PORTRAIT_ATTR((i - 2) * 2 + 1), 1);
        }
      }
      SWITCH_BACK();
    }
  }
}
