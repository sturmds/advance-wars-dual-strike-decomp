#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/unit_show_task.h"
#include "game/sprite_anim.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba4.h"

void func_020048f4(u16 frames);
BOOL Screens_IsTransitionRunning(void);
void func_02004284(int a0, Ov005Task* task);
void func_0202a7c0(void);
void Sound_PlaySe(u32 soundId);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
s32 HwDiv32(s32 numerator, s32 denominator);
void SpriteAnim_SetAnimation(ShowSprite* sprite, int frame);
void Task_ResumeScript(Ov005Task* task);
void ModeFont_LoadHere(int tile, int paletteSlot);
const void* Text_Get(u32 textId);
void ModeFont_StartTextTask(const void* text, int tile, int paletteSlot,
                            int unk_56, u8 unk_5c, int unk_58, int unused);
BOOL Ov005_StateIsEnabled(Unk_ov005_0236bba4* work);
void Ov005_RunState(Unk_ov005_0236bba4* work);
void Ov005_UpdateStateScreenButtons(Unk_ov005_0236bba4* work, int scroll);
void Ov005_RemoveStateScreenButtons(Unk_ov005_0236bba4* work);
void func_ov005_0235ec40(Unk_ov005_0236bba4* work);

extern Keypad* gKeypad;
void Ov005StateScreen_OnNextTouched(void);

/* "039" */
char data_ov005_0236a630[4] = {
#include "data/ov005/data_ov005_0236a630.inc.c"
};
MenuButtonTemplate data_ov005_0236a634 = {
#include "data/ov005/data_ov005_0236a634.inc.c"
};
extern const MenuButtonTemplate data_ov005_0236a684;
extern const MenuButtonTemplate data_ov005_0236a6ac;

#define LAST_STATE 40

/* work->state is u32 in the shared header; the (int)
 * casts give the signed compares and shifts the original uses. */

/* Per-frame update of the state screen (Ov005_SetUpStateScreen). Left and
 * Right, or buttons that set prevTouched and nextTouched, step to the previous or next
 * enabled state, wrapping over 0..40; both screens darken over 16 frames,
 * Ov005_RunState draws the new state, and they brighten again. R (or rTouched)
 * starts a scroll mode when the state allows it: held Left and Right move
 * BG layers 2 and 3 one pixel per frame within 0..scrollMax, and B returns.
 * B (or bTouched) on the state view fades out, hides both screens and resumes
 * the task script. Outside scroll mode the state's number is drawn with the
 * three unk_390 sprites. */
void Ov005_UpdateStateScreen(Ov005Task* task) {
  Unk_027c0280* saved0280;
  int tens;
  Unk_ov005_0236bba4* work;
  Unk_ov005_0236bba4_State* state;
  Unk_027c0280*
      digitSaved0280; /* needed to match: own variables for the digit block */
  Unk_02080440* digitSaved027c;
  Unk_02080440* saved027c;
  int x;
  int evy;
  int number;
  work = task->work;

  switch (task->step) {
    case 0:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      data_027c0280->blendEvy = 8;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      func_020048f4(32);
      task->step++;
    case 1:
      if (!Screens_IsTransitionRunning()) {
        task->step = 10;
      }
      break;
    case 10:
      if ((gKeypad->pressed & PAD_KEY_LEFT) || work->prevTouched == 1) {
        do {
          if ((int)work->state > 0) {
            work->state--;
          } else {
            work->state = LAST_STATE;
          }
        } while (!Ov005_StateIsEnabled(work));
        Sound_PlaySe(0x10002);
        task->step = 20;
        break;
      }
      if ((gKeypad->pressed & PAD_KEY_RIGHT) || work->nextTouched == 1) {
        do {
          if ((int)work->state < LAST_STATE) {
            work->state++;
          } else {
            work->state = 0;
          }
        } while (!Ov005_StateIsEnabled(work));
        Sound_PlaySe(0x10002);
        task->step = 20;
        break;
      }
      if (work->wallLocked == 0 &&
          ((gKeypad->pressed & PAD_BUTTON_R) || work->rTouched == 1)) {
        Sound_PlaySe(0x10006);
        task->step = 30;
        break;
      }
      if ((gKeypad->pressed & PAD_BUTTON_B) || work->bTouched == 1) {
        MenuButtons_RemoveAll();
        Sound_PlaySe(0x10009);
        task->step = 100;
      }
      break;
    case 20:
      task->frame = 0;
      task->frames = 16;
      task->step++;
    case 21:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      evy = Math_Ease(0, 0x8000, 0x10000, task->frame, task->frames);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      data_027c0280->blendEvy = evy >> 12;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      evy = Math_Ease(0, 0, 0x10000, task->frame, task->frames);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      data_027c0280->blendEvy = evy >> 12;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step++;
      }
      break;
    case 22:
      work->scrollX = 0;
      work->scrollY = 0;
      work->scrollStepX = 0;
      work->unk_28 = 0;
      Ov005_RunState(work);
      task->step++;
      break;
    case 23:
      if (work->wallLocked == 1 && work->prevWallLocked == 0) {
        MenuButton_Remove(241);
      }
      if (work->wallLocked == 0 && work->prevWallLocked == 1) {
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        MenuButton_Add(0, 0, &data_ov005_0236a6ac, 241);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
      }
      task->frame = 0;
      task->frames = 16;
      task->step++;
      break;
    case 24:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      evy = Math_Ease(0, 0x10000, 0x8000, task->frame, task->frames);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      data_027c0280->blendEvy = evy >> 12;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      evy = Math_Ease(0, 0x10000, 0, task->frame, task->frames);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      data_027c0280->blendEvy = evy >> 12;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step++;
      }
      break;
    case 25:
      func_ov005_0235ec40(work);
      task->step = 10;
      break;
    case 30:
      func_ov005_0235ec40(work);
      work->scrollStepX = 0;
      work->unk_28 = 0;
      MenuButton_Remove(243);
      MenuButton_Remove(244);
      MenuButton_Remove(241);
      work->scrollMode = 1;
      func_0202a7c0();
      work->button245Shown = 0;
      work->button246Shown = 0;
      task->step++;
    case 31:
      if ((gKeypad->held & PAD_KEY_LEFT) || work->leftTouched == 1) {
        work->scrollStepX = -0x1000;
      } else if ((gKeypad->held & PAD_KEY_RIGHT) || work->rightTouched == 1) {
        work->scrollStepX = 0x1000;
      }
      if ((gKeypad->pressed & PAD_BUTTON_B) || work->bTouched != 0) {
        Sound_PlaySe(0x10009);
        task->step++;
        break;
      }
      work->scrollX += work->scrollStepX;
      work->scrollStepX = 0;
      state = &work->stateTable[work->state];
      if (work->scrollX < 0) {
        work->scrollX = 0;
      }
      if (work->scrollX > state->scrollMax) {
        work->scrollX = state->scrollMax;
      }
      Ov005_UpdateStateScreenButtons(work, work->scrollX >> 12);
      break;
    case 32:
      func_ov005_0235ec40(work);
      Ov005_RemoveStateScreenButtons(work);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      MenuButton_Add(0, 0, &data_ov005_0236a684, 243);
      MenuButton_Add(0, 0, &data_ov005_0236a634, 244);
      MenuButton_Add(0, 0, &data_ov005_0236a6ac, 241);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      work->scrollMode = 0;
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      ModeFont_LoadHere(0x80, 2);
      ModeFont_StartTextTask(Text_Get(0x7d00001b), 0x80, 2, 0, 0, 0, 0);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      task->step = 10;
      break;
    case 100:
      func_02004284(32, task);
      task->step++;
      break;
    case 101:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      func_0202a7c0();
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
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
      task->step++;
      break;
    case 102:
      Task_ResumeScript(task);
      return;
  }

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = work->scrollY >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = work->scrollX >> 12;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = work->scrollY >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = work->scrollX >> 12;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = work->scrollY >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = work->scrollX >> 12;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = work->scrollY >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = work->scrollX >> 12;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  if (work->scrollMode != 0) {
    return;
  }
  digitSaved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  digitSaved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  number = work->stateTable[work->state].number;
  tens = HwDiv32(number, 10);
  SpriteAnim_SetAnimation(work->unk_390[0], 10);
  SpriteAnim_SetAnimation(work->unk_390[1], tens);
  SpriteAnim_SetAnimation(work->unk_390[2], number - tens * 10);
  x = 96;
  SpriteAnim_Draw(work->unk_390[0], x, 64);
  x += 42;
  SpriteAnim_Draw(work->unk_390[1], x, 64);
  switch (tens) {
    case 0:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      x += 14;
      break;
    case 1:
      x += 8;
      break;
  }
  SpriteAnim_Draw(work->unk_390[2], x, 64);
  Screen_SetCurrent(digitSaved027c);
  Screen_SetCurrentGfx(digitSaved0280);
}
