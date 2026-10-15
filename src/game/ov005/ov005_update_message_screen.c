#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/text_printer.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb8c.h"
#include "game/brightness_fade.h"
#include "game/math.h"
#include "game/screen_other_map.h"

BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void Screens_BlackOut(void);
Task* func_020fcb08(int arg);
BOOL BrightnessFade_IsActive(Task* task);
void func_020fccb0(Task* task);
void Task_ResumeScript(Ov005Task* task);
TextPrinter* Text_PrintByIdOnLayer0(int x, int y, u16* map, u32 textId,
                                    u16 unk_30, u16 unk_38);
BOOL Text_IsPrinting(void);
void Text_StopPrinting(void);

void func_ov005_02358954(void);
void Ov005MessageScreen_OnButtonConfirm(void);

char gRikiishiFile02c[] = {
#include "data/ov005/gRikiishiFile02c.inc.c"
};
char gRikiishiFile02b[] = {
#include "data/ov005/gRikiishiFile02b.inc.c"
};
char gRikiishiFile02a[] = {
#include "data/ov005/gRikiishiFile02a.inc.c"
};
char gRikiishiFile015_2[] = {
#include "data/ov005/gRikiishiFile015_2.inc.c"
};
char gRikiishiFile014_2[] = {
#include "data/ov005/gRikiishiFile014_2.inc.c"
};
char gRikiishiFile013_2[] = {
#include "data/ov005/gRikiishiFile013_2.inc.c"
};
char gRikiishiFile01b_2[] = {
#include "data/ov005/gRikiishiFile01b_2.inc.c"
};
char gRikiishiFile019[] = {
#include "data/ov005/gRikiishiFile019.inc.c"
};
char gRikiishiFile01a[] = {
#include "data/ov005/gRikiishiFile01a.inc.c"
};
char gRikiishiFile016_2[] = {
#include "data/ov005/gRikiishiFile016_2.inc.c"
};
char data_ov005_02369478[] = {
#include "data/ov005/data_ov005_02369478.inc.c"
};
char data_ov005_02369474[] = {
#include "data/ov005/data_ov005_02369474.inc.c"
};
char data_ov005_02369470[] = {
#include "data/ov005/data_ov005_02369470.inc.c"
};
/* layer 2 palette */
char gRikiishiFile038[] = {
#include "data/ov005/gRikiishiFile038.inc.c"
};
/* layer 2 map */
char gRikiishiFile037[] = {
#include "data/ov005/gRikiishiFile037.inc.c"
};
/* layer 2 tiles */
char gRikiishiFile036[] = {
#include "data/ov005/gRikiishiFile036.inc.c"
};
/* layer 2 palette */
char data_ov005_02369460[] = {
#include "data/ov005/data_ov005_02369460.inc.c"
};
/* layer 2 map */
char data_ov005_0236945c[] = {
#include "data/ov005/data_ov005_0236945c.inc.c"
};
/* layer 2 tiles */
char data_ov005_02369458[] = {
#include "data/ov005/data_ov005_02369458.inc.c"
};
char data_ov005_02369454[] = {
#include "data/ov005/data_ov005_02369454.inc.c"
};
char data_ov005_02369450[] = {
#include "data/ov005/data_ov005_02369450.inc.c"
};
char data_ov005_0236944c[] = {
#include "data/ov005/data_ov005_0236944c.inc.c"
};
/* confirm: Ov005MessageScreen_OnButtonConfirm */
static MenuButtonTemplate gOv005MessageScreenButton = {
#include "data/ov005/gOv005MessageScreenButton.inc.c"
};

#define MESSAGE_BUTTON_ID 0xfd

/* Per-frame update of an overlay 5 screen (task script data_ov005_023694cc):
 * fade both screens in (64 frames), wait 32 frames, alpha-blend the sub
 * screen's first target in over 64 frames, print a message on the main
 * screen's layer 0, then show a button and wait for A or the button. Then
 * fade out, stop the song task, hide every layer and OBJ on both screens,
 * fade out the song and resume the task script. */
void Ov005_UpdateMessageScreen(Ov005Task* task) {
  Unk_ov005_0236bb8c* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  TextPrinter* printer;
  int eva;

  switch (task->step) {
    case 0:
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
      task->step = 2;
      return;
    case 2:
      task->frame = 0;
      task->frames = 32;
      task->step++;
    case 3:
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step++;
      }
      return;
    case 4:
      task->frame = 0;
      task->frames = 64;
      task->step++;
    case 5:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      eva = Math_Ease(0, 0, 16, task->frame, task->frames);
      data_027c0280->blendMode = BLEND_MODE_ALPHA;
      data_027c0280->blendEva = eva;
      data_027c0280->blendEvb = 16 - eva;
      data_027c0280->blendEvy = 0;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step = 10;
      }
      return;
    case 10:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      printer = Text_PrintByIdOnLayer0(2, 4, data_027c0280->layer0Map,
                                       0x21000003, 0, 0x41);
      work->textPrinter = printer;
      printer->charDelay = 6;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      task->step++;
    case 11:
      if (!Text_IsPrinting()) {
        task->step++;
      }
      return;
    case 12:
      MenuButton_Add(0, 0, &gOv005MessageScreenButton, MESSAGE_BUTTON_ID);
      MenuButton_Select(MESSAGE_BUTTON_ID);
      work->confirmed = 0;
      task->step++;
    case 13:
      if (gKeypad->pressed & PAD_BUTTON_A) {
        task->step = 100;
        return;
      }
      if (work->confirmed) {
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
      func_020fccb0(work->songTask);
      Screens_BlackOut();
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt &= ~0x200;
      data_027c0280->dispCnt &= ~0x400;
      data_027c0280->dispCnt &= ~0x800;
      data_027c0280->dispCnt &= ~0x1000;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt &= ~0x200;
      data_027c0280->dispCnt &= ~0x400;
      data_027c0280->dispCnt &= ~0x800;
      data_027c0280->dispCnt &= ~0x1000;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      func_020fd618(work->fadeTask);
      func_020fd618(work->fadeTask2);
      MenuButton_Remove(MESSAGE_BUTTON_ID);
      Text_StopPrinting();
      task->step++;
      return;
    case 102:
      work->songTask = func_020fcb08(1);
      task->step++;
    case 103:
      if (BrightnessFade_IsActive(work->songTask)) {
        return;
      }
      func_020fccb0(work->songTask);
      Task_ResumeScript(task);
      return;
  }
}
