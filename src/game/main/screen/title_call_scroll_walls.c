#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/touch.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void func_020517d8(void);

#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Last title call task script step (data_0215bc10), run every frame. Once
 * the fade-in is over (evy 0), A, B, Start or a touch starts the
 * data_0215bbe8 task (func_020517d8). Every fourth frame, on both screens,
 * the wall layers 1 and 2 scroll one pixel. */
void TitleCall_ScrollWalls(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  if (data_027c0280->blendEvy == 0) {
    if ((gKeypad->pressed & (PAD_BUTTON_A | PAD_BUTTON_B | PAD_BUTTON_START)) ||
        (gTouch.phase == TOUCH_PRESSED && gTouch.active == 1)) {
      func_020517d8();
    }
  }
  if ((FRAME(task) & 3) == 0) {
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c0280->bgScrollX[data_027c0280->layer1Bg]--;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] =
        data_027c0280->bgScrollX[data_027c0280->layer1Bg];
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c0280->bgScrollX[data_027c0280->layer1Bg]--;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] =
        data_027c0280->bgScrollX[data_027c0280->layer1Bg];
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  FRAME(task)++;
}
