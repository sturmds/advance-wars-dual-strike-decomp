#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/ov005_task.h"
#include "game/screen_other_map.h"
#include "game/text_printer.h"
#include "game/Unk_ov005_0236bb80.h"
#include "game/brightness_fade.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void Screens_BlackOut(void);
void TextPrinter_ShowAll(TextPrinter* printer);
BOOL Text_IsPrinting(void);
void Text_StopPrinting(void);
void Task_Destroy(Task* task);
void Task_ResumeScript(Ov005Task* task);

extern const TaskScriptCmd data_ov005_02368048[];
extern const MenuButtonTemplate data_ov005_02368070;

/* Per-frame update. Every frame it eases the x scroll of layers 1 and 2 on
 * screen data_027c0288.unk_04 (0 to 120 and 256 to 376 pixels). Steps: start
 * the BG wave task (Ov005_UpdateBgWave) and fade in (64 frames); wait until
 * the text is printed, or on A call TextPrinter_ShowAll on the text printer and go on;
 * add button 253 and wait for A or the button (its callback
 * func_ov005_023536a4 sets unk_08); fade out, hide all layers on both
 * screens, remove the button, the wave task and the text, and resume the
 * task script. */
void Ov005_UpdateSlideInTextScreen(Ov005Task* task) {
  Unk_ov005_0236bb80* work = task->work;
  TextPrinter* textPrinter = work->textPrinter;
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* savedScreen;
  int x1;
  int x2;
  Task* waveTask;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  x1 = Math_Ease(0, 0, 0x78000, work->unk_00, work->unk_04);
  x2 = Math_Ease(0, 0x100000, 0x178000, work->unk_00, work->unk_04);
  /* the compare is signed: unk_00 counts frames up to unk_04 */
  if ((int)work->unk_00 < work->unk_04) {
    work->unk_00++;
  }
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = x1 >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = x2 >> 12;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  switch (task->step) {
    case 0:
      waveTask = Task_Create(data_ov005_02368048, task);
      ((Ov005Task*)waveTask)->work = work;
      work->waveTask = waveTask;
      work->unk_28 = FALSE;
      work->fadeTask = func_020fcdc8(task, 64);
      work->fadeTask2 = func_020fcda0(task, 64);
      task->step++;
    case 1:
      if (func_020fd624(work->fadeTask)) {
        return;
      }
      if (func_020fd624(work->fadeTask2)) {
        return;
      }
      func_020fd618(work->fadeTask);
      func_020fd618(work->fadeTask2);
      task->step = 10;
      return;
    case 10:
      if (gKeypad->pressed & PAD_BUTTON_A) {
        TextPrinter_ShowAll(textPrinter);
        task->step++;
        return;
      }
      if (!Text_IsPrinting()) {
        task->step++;
      }
      return;
    case 11:
      MenuButton_Add(0, 0, &data_ov005_02368070, 253);
      MenuButton_Select(253);
      work->unk_08 = FALSE;
      task->step++;
    case 12:
      if (gKeypad->pressed & PAD_BUTTON_A) {
        task->step = 100;
        return;
      }
      if (work->unk_08) {
        task->step = 100;
      }
      return;
    case 100:
      work->fadeTask = BrightnessFade_StartOutBlackScreen0(task, 64);
      work->fadeTask2 = func_020fcd50(task, 64);
      task->step++;
      return;
    case 101:
      if (func_020fd624(work->fadeTask)) {
        return;
      }
      if (func_020fd624(work->fadeTask2)) {
        return;
      }
      Screens_BlackOut();
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      GFX_DISPCNT(data_027c0280)->bg0 = 0;
      GFX_DISPCNT(data_027c0280)->bg1 = 0;
      GFX_DISPCNT(data_027c0280)->bg2 = 0;
      GFX_DISPCNT(data_027c0280)->bg3 = 0;
      GFX_DISPCNT(data_027c0280)->obj = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      GFX_DISPCNT(data_027c0280)->bg0 = 0;
      GFX_DISPCNT(data_027c0280)->bg1 = 0;
      GFX_DISPCNT(data_027c0280)->bg2 = 0;
      GFX_DISPCNT(data_027c0280)->bg3 = 0;
      GFX_DISPCNT(data_027c0280)->obj = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      func_020fd618(work->fadeTask);
      func_020fd618(work->fadeTask2);
      MenuButton_Remove(253);
      Task_Destroy(work->waveTask);
      Text_StopPrinting();
      task->step = 102;
      return;
    case 102:
      Task_ResumeScript(task);
      return;
  }
}
