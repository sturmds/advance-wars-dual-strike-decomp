#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb80.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
BOOL Screens_IsTransitionRunning(void);
void func_02004d3c(int a);
void func_02004d64(int a);
BOOL Text_IsPrinting(void);
void Text_StopPrinting(void);
void Task_ResumeScript(Ov005Task* task);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuButtonTemplate data_ov005_02368070;

/* Like Ov005_UpdateTextPromptScreen, but the sub screen's BG layers 1 and 2
 * scroll vertically each frame by a linear tween (0 to 192 and 256 to 448,
 * over unk_04 frames), and no song fade at the end. */
void Ov005_UpdateScrollingTextPromptScreen(Ov005Task* task) {
  Unk_ov005_0236bb80* work = task->work;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int scrollY1;
  int scrollY2;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  scrollY1 = Math_Ease(0, 0, 0xc0000, work->unk_00, work->unk_04);
  scrollY2 = Math_Ease(0, 0x100000, 0x1c0000, work->unk_00, work->unk_04);
  /* needed to match: signed compare of the frame counter */
  if ((int)work->unk_00 < work->unk_04) {
    work->unk_00++;
  }
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = scrollY1 >> 12;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = scrollY2 >> 12;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  switch (task->step) {
    case 0:
      func_02004d3c(64);
      task->step++;
    case 1:
      if (!Screens_IsTransitionRunning()) {
        task->step = 10;
      }
      return;
    case 10:
      if (!Text_IsPrinting()) {
        task->step++;
      }
      return;
    case 11:
      MenuButton_Add(0, 0, &data_ov005_02368070, 253);
      MenuButton_Select(253);
      work->unk_08 = 0;
      task->step++;
    case 12:
      if (gKeypad->pressed & PAD_BUTTON_A) {
        task->step = 100;
        return;
      }
      if (work->unk_08 != 0) {
        task->step = 100;
      }
      return;
    case 100:
      func_02004d64(64);
      task->step++;
      return;
    case 101:
      if (Screens_IsTransitionRunning()) {
        return;
      }
      MenuButton_Remove(253);
      Text_StopPrinting();
      Task_ResumeScript(task);
      return;
  }
}
