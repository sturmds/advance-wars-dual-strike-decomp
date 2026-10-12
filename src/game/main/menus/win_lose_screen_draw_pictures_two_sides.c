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

/* Draws the CO pictures of the two-side win/lose screen in place: side A
 * from x 24 on the second screen (y 192), side B flipped from x 232
 * leftwards on the main screen (y 48), 32 pixels apart. */
void WinLoseScreen_DrawPicturesTwoSides(WinLoseTask* task) {
  int i;

  SWITCH_TO(SECOND_MAP);
  for (i = 0; i < task->portraitsA; i++) {
    Co_QueuePictureSprite((24 + i * 32) & 0x1ff, 192, PORTRAIT_ATTR(i), i);
  }
  SWITCH_BACK();

  SWITCH_TO(MAIN_MAP);
  for (i = 0; i < task->portraitsB; i++) {
    Co_QueuePictureSprite(((232 - i * 32) & 0x1ff) | OBJ_HFLIP, 48,
                          PORTRAIT_ATTR(i), i);
  }
  SWITCH_BACK();
}
