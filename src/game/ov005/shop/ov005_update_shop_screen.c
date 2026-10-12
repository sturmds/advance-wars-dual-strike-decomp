#include "types.h"
#include "game/event_script.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/ov005_task.h"
#include "game/play_records.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov005_0236bbb0.h"

void func_020048f4(u16 frames);
void func_0200493c(u16 frames);
BOOL Screens_IsTransitionRunning(void);
void func_0200270c(int x, int y, int attr, int list, int hFlip);
void Bg_FillRect(void* map, int x, int y, int width, int height, int tile);
void EventTextMap_QueueCopy(void);
void func_0201c898(void);
int func_02014c24(void);
BOOL PlayRecords_SpendPoints(u32 points);
void EventSequenceTask_StartWithSongFade(Ov005Task* task);
Task* func_020fcb08(int song);
BOOL BrightnessFade_IsActive(Task* task);
void func_020fccb0(Task* task);
BOOL Touch_IsPressed(void);
BOOL Keypad_ConfirmNextPressed(void);
u32 Rng_Next(void);
void Sound_PlaySe(u32 soundId);
void Sound_PlaySubSe(u32 soundId);
void Sound_StopSubSe(int fadeFrames);
void Gfx_RequestLayerCopy(int layer);
void Mem_Copy32(const void* src, void* dst, u32 size);
s32 HwDiv32(s32 numerator, s32 denominator);
void Wallpaper_DestroyTasks(void);
void Screen1_DestroyTextTasks(void);
void Task_ResumeScript(Ov005Task* task);
void Ov005_StartEventScriptTask(const EventScriptCommand* script, void* parent);
void Ov005_RemoveShopButtons(Unk_ov005_0236bbb0* work);
int Ov005_GetSelectedShopItemIndex(Unk_ov005_0236bbb0* work);
void Ov005_BuySelectedShopItem(Unk_ov005_0236bbb0* work);
void Ov005_CollectShopItems(Unk_ov005_0236bbb0* work);
void Ov005_DrawShopPoints(Unk_ov005_0236bbb0* work);
void Ov005_DrawShopPage(Unk_ov005_0236bbb0* work);
int Ov005_GetShopItemsFromPage(
    Unk_ov005_0236bbb0* work); /* items from the current page on */
BOOL Ov005_IsShopSoldOut(void);
u8 Ov005_GetShopVisits(void);
void Ov005_AddShopVisit(void);
int Ov005_IsShopScriptRunning(void);
void func_ov005_02367ec0(const EventScriptCommand* script);
void Ov005_AddShopPageButtons(
    Unk_ov005_0236bbb0* work); /* adds the page's buttons */
void Ov005_DrawShopPageNumber(
    Unk_ov005_0236bbb0* work); /* draws the page numbers */

extern u16 gEventChoiceAnswer; /* answer of the yes/no question, 0: yes */
extern u8 gEventTextMap[];
extern u8 data_022978e0[];
extern const TaskScriptCmd data_ov005_0236b35c[];
extern const TaskScriptCmd data_ov005_0236b374[];
extern const MenuButtonTemplate data_ov005_0236b3b4;
extern const MenuButtonTemplate data_ov005_0236b404;
extern const MenuButtonTemplate data_ov005_0236b42c;
extern const EventScriptCommand* const data_ov005_0236b454[17];
extern const EventScriptCommand data_ov005_0236b4f0[];
extern const EventScriptCommand data_ov005_0236b520[];
extern const EventScriptCommand data_ov005_0236b550[];
extern const EventScriptCommand data_ov005_0236b780[];
extern const EventScriptCommand data_ov005_0236b840[];
extern const EventScriptCommand data_ov005_0236b880[];
extern const EventScriptCommand data_ov005_0236b8c0[];
extern const EventScriptCommand data_ov005_0236b980[];
extern const EventScriptCommand data_ov005_0236b9c0[];
extern const EventScriptCommand data_ov005_0236ba00[];

#define SWITCH_TO(state)       \
  saved0280 = data_027c0280;   \
  Screen_SetCurrentGfx(state); \
  saved027c = data_027c027c;   \
  Screen_SetCurrent(state)

#define SWITCH_BACK()           \
  Screen_SetCurrent(saved027c); \
  Screen_SetCurrentGfx(saved0280)

/* Per-frame update of Hachi's shop (set up by Ov005_SetUpShopScreen).
 * Hachi greets (one script on the first visit, a random one of 17 after,
 * another when nothing is left), then the list runs: L/Y/Left and
 * R/X/Right turn the page, A asks whether to buy the item, B leaves (the
 * touch buttons set flags that do the same). Buying counts
 * the points down on screen and removes the item from the list. On
 * leaving, Hachi says goodbye (one of four scripts by visit count), the
 * screens fade out and the task script resumes. Every frame ends by
 * drawing the page numbers, the points and the page. */
void Ov005_UpdateShopScreen(Ov005Task* task) {
  Unk_ov005_0236bbb0* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u16 keys;

  switch (task->step) {
    case 0:
      func_020048f4(32);
      task->step++;
    case 1:
      if (!Screens_IsTransitionRunning()) {
        work->redrawRows = 0;
        task->step = 12;
      }
      break;
    case 12: /* greeting */
      if (Ov005_IsShopSoldOut()) {
        Ov005_StartEventScriptTask(data_ov005_0236b9c0, task);
        task->step++;
      } else if (Ov005_GetShopVisits() == 0) {
        Ov005_StartEventScriptTask(data_ov005_0236ba00, task);
        task->step++;
      } else {
        Ov005_StartEventScriptTask(data_ov005_0236b454[Rng_Next() % 17], task);
        task->step++;
      }
      break;
    case 13:
      if (!Ov005_IsShopScriptRunning()) {
        func_02014c24();
        task->step++;
      }
      break;
    case 14:
      work->redrawRows = 1;
      Task_Create(data_ov005_0236b35c, task);
      task->step++;
    case 15:
      if (work->transition == 0) {
        SWITCH_TO(data_027c0288.unk_00);
        MenuButton_Add(0, 0, &data_ov005_0236b42c, 240);
        MenuButton_Add(0, 0, &data_ov005_0236b404, 241);
        MenuButton_Add(0, 0, &data_ov005_0236b3b4, 242);
        SWITCH_BACK();
        Ov005_AddShopPageButtons(work);
        task->step = 20;
      }
      break;
    case 20: /* list input; with nothing for sale only B works */
      if (Ov005_IsShopSoldOut()) {
        if ((gKeypad->pressed & PAD_BUTTON_B) || work->leaveRequested == 1) {
          Sound_PlaySe(0x10009);
          work->leaveRequested = 0;
          task->step = 90;
        }
        break;
      }
      keys = gKeypad->pressed;
      if ((keys & PAD_BUTTON_A) || work->buyRequested == 1) {
        Sound_PlaySe(0x10006);
        work->buyRequested = 0;
        Ov005_RemoveShopButtons(work);
        MenuButtons_DisableTouch();
        task->step = 30;
      } else if ((keys & PAD_BUTTON_B) || work->leaveRequested == 1) {
        Sound_PlaySe(0x10009);
        work->leaveRequested = 0;
        Ov005_RemoveShopButtons(work);
        MenuButtons_DisableTouch();
        task->step = 90;
      } else if ((keys & (PAD_BUTTON_L | PAD_BUTTON_Y)) ||
                 (gKeypad->repeat & PAD_KEY_LEFT) ||
                 work->prevPageRequested == 1) {
        work->prevPageRequested = 0;
        if (work->page <= 0) {
          work->page = work->lastPage;
        } else {
          work->page--;
        }
        work->cursorRow = work->savedCursorRow;
        work->drawnCursorRow = -1;
        Ov005_RemoveShopButtons(work);
        Ov005_AddShopPageButtons(work);
      } else if ((keys & (PAD_BUTTON_R | PAD_BUTTON_X)) ||
                 (gKeypad->repeat & PAD_KEY_RIGHT) ||
                 work->nextPageRequested == 1) {
        work->nextPageRequested = 0;
        if (work->page >= work->lastPage) {
          work->page = 0;
        } else {
          work->page++;
        }
        work->cursorRow = work->savedCursorRow;
        work->drawnCursorRow = -1;
        Ov005_RemoveShopButtons(work);
        Ov005_AddShopPageButtons(work);
      }
      break;
    case 30: /* A: ask whether to buy */
      work->highlightCursorRow = 1;
      work->showCursor = 1;
      Task_Create(data_ov005_0236b374, task);
      task->step++;
    case 31:
      if (work->transition == 0) {
        func_ov005_02367ec0(
            work->items[Ov005_GetSelectedShopItemIndex(work)].confirmScript);
        task->step++;
      }
      break;
    case 32:
      if (!Ov005_IsShopScriptRunning()) {
        if (gEventChoiceAnswer == 0) {
          if (PlayRecords_SpendPoints(
                  work->items[Ov005_GetSelectedShopItemIndex(work)].price)) {
            task->step = 40;
          } else {
            task->step = 35;
          }
        } else {
          task->step++;
        }
      }
      break;
    case 33: /* answered no */
      func_ov005_02367ec0(data_ov005_0236b550);
      task->step++;
    case 34:
      if (!Ov005_IsShopScriptRunning()) {
        task->step = 50;
      }
      break;
    case 35: /* not enough points */
      func_ov005_02367ec0(data_ov005_0236b520);
      task->step++;
    case 36:
      if (!Ov005_IsShopScriptRunning()) {
        task->step = 50;
      }
      break;
    case 40: /* bought: count the points down, 10 every 2 frames */
      func_ov005_02367ec0(data_ov005_0236b980);
      Sound_PlaySubSe(0x10005);
      task->frame = 0;
      task->frames = 0;
      task->step++;
    case 41:
      task->frame++;
      if (task->frame == 2 && task->frames == 0) {
        task->frame = 0;
        work->points -= 10;
      }
      /* frames: 1 once the count is done (or skipped by a key) */
      if (task->frames == 0) {
        if (Touch_IsPressed() || Keypad_ConfirmNextPressed() ||
            work->points <= gPlayRecords.points) {
          task->frames = 1;
          work->points = gPlayRecords.points;
          Sound_StopSubSe(0);
          Sound_PlaySe(0x10004);
        }
      }
      if (!Ov005_IsShopScriptRunning() && work->points <= gPlayRecords.points) {
        SWITCH_TO(data_027c0288.unk_00);
        Bg_FillRect(gEventTextMap, 6, 0, 26, 6, 0);
        EventTextMap_QueueCopy();
        SWITCH_BACK();
        work->points = gPlayRecords.points;
        work->boughtSomething = 1;
        Ov005_BuySelectedShopItem(work);
        work->itemCount--;
        work->lastPage = HwDiv32(work->itemCount - 1, 6);
        if (work->itemCount != 0) {
          if (Ov005_GetShopItemsFromPage(work) == 0) {
            work->cursorRow = 0;
            work->page = work->lastPage;
          }
        }
        Ov005_CollectShopItems(work);
        work->highlightCursorRow = 0;
        Gfx_RequestLayerCopy(0);
        task->step++;
      }
      break;
    case 42:
      if (!Ov005_IsShopSoldOut()) {
        task->step = 50;
        break;
      }
      work->showCursor = 0;
      task->step++;
      break;
    case 43: /* sold out */
      func_ov005_02367ec0(data_ov005_0236b4f0);
      task->step++;
    case 44:
      if (!Ov005_IsShopScriptRunning()) {
        task->step = 93;
      }
      break;
    case 50: /* back to the list */
      Task_Create(data_ov005_0236b35c, task);
      task->step++;
    case 51:
      if (work->transition == 0) {
        work->highlightCursorRow = 0;
        work->showCursor = 0;
        Ov005_AddShopPageButtons(work);
        MenuButtons_EnableTouch();
        work->buyRequested = 0;
        work->leaveRequested = 0;
        work->prevPageRequested = 0;
        work->nextPageRequested = 0;
        task->step = 20;
      }
      break;
    case 90: /* B: goodbye */
      Task_Create(data_ov005_0236b374, task);
      task->step++;
    case 91:
      if (work->transition == 0) {
        if ((Ov005_GetShopVisits() & 3) == 0) {
          func_ov005_02367ec0(data_ov005_0236b880);
        } else if ((Ov005_GetShopVisits() & 3) == 1) {
          func_ov005_02367ec0(data_ov005_0236b840);
        } else if ((Ov005_GetShopVisits() & 3) == 2) {
          func_ov005_02367ec0(data_ov005_0236b780);
        } else {
          func_ov005_02367ec0(data_ov005_0236b8c0);
        }
        task->step++;
      }
      break;
    case 92:
      if (!Ov005_IsShopScriptRunning()) {
        task->step++;
      }
      break;
    case 93:
      Task_Create(data_ov005_0236b35c, task);
      task->step++;
    case 94:
      if (work->transition == 0) {
        task->step++;
        break;
      }
      SWITCH_TO(data_027c0288.unk_00);
      Mem_Copy32(gEventTextMap, data_027c0280->layer0Map + 0x240, 0x180);
      Gfx_RequestLayerCopy(0);
      Mem_Copy32(data_022978e0, data_027c0280->layer1Map + 0x240, 0x180);
      Gfx_RequestLayerCopy(1);
      SWITCH_BACK();
      task->step = 100;
      break;
    case 100: /* fade out and leave */
      func_0201c898();
      Ov005_AddShopVisit();
      MenuButtons_RemoveSelectable();
      MenuButtons_RemoveUnselectable();
      if (work->boughtSomething == 0) {
        work->songTask = func_020fcb08(1);
      }
      func_0200493c(32);
      task->step++;
    case 101:
      if (!Screens_IsTransitionRunning()) {
        task->step++;
      }
      break;
    case 102:
      Wallpaper_DestroyTasks();
      Screen1_DestroyTextTasks();
      SWITCH_TO(data_027c0288.unk_04);
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt &= ~0x200;
      data_027c0280->dispCnt &= ~0x400;
      data_027c0280->dispCnt &= ~0x800;
      data_027c0280->dispCnt &= ~0x1000;
      SWITCH_BACK();
      SWITCH_TO(data_027c0288.unk_00);
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt &= ~0x200;
      data_027c0280->dispCnt &= ~0x400;
      data_027c0280->dispCnt &= ~0x800;
      data_027c0280->dispCnt &= ~0x1000;
      SWITCH_BACK();
      task->step++;
      break;
    case 103:
      if (work->boughtSomething != 0) {
        EventSequenceTask_StartWithSongFade(task);
        Task_ResumeScript(task);
        return;
      }
      task->step++;
      break;
    case 104:
      if (!BrightnessFade_IsActive(work->songTask)) {
        func_020fccb0(work->songTask);
        Task_ResumeScript(task);
        return;
      }
      break;
  }

  if (work->cursorRow != work->drawnCursorRow) {
    Sound_PlaySe(0x10002);
  }
  work->drawnCursorRow = work->cursorRow;
  SWITCH_TO(data_027c0288.unk_04);
  func_0200270c(200, 160, 0xaa00, 0, 0);
  SWITCH_BACK();
  Ov005_DrawShopPageNumber(work);
  Ov005_DrawShopPoints(work);
  Ov005_DrawShopPage(work);
}
