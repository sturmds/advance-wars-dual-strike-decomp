#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/icon.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bba0.h"

void Sound_PlaySe(u32 soundId);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Screen_SetCurrentBoth(void* state);
void Screen1_DestroyTextTasks(void);
void MenuButtons_RemoveUnselectable(void);
void Task_ResumeScript(Ov005Task* task);
BOOL Screens_IsTransitionRunning(void);
void func_020048f4(u16 frames);
void func_0200493c(u16 frames);
void HelpBand_Stop(void);
void Wallpaper_LoadBothOnLayer3(int index);
void EventSequenceTask_Start(Ov005Task* task);
Task* PaletteFade_StartToBlackScreen1(Ov005Task* parent, int frames, int a,
                                      int b);
Task* PaletteFade_StartFromBlackScreen1(Ov005Task* parent, const void* palettes,
                                        int frames, int a, int b);
void Screens_BlackOut(void);
void func_020fd618(Task* task);
void Ov005_AddCoListButtons(Ov005Task* task);
void Ov005_EnableCoListTouch(Ov005Task* task);
void Ov005_DisableCoListTouch(Ov005Task* task);
void Ov005_UpdateScrollButtons(Ov005Task* task, int scroll);
void Ov005_RemoveScrollButtons(Ov005Task* task);
u8 Ov005_SaveCoListEntry(int entry);
int Ov005_ClearCoListTouchFlags(Ov005Task* task);

/* CO id of each list entry */
u8 data_ov005_0236a48c[33] = {
#include "data/ov005/data_ov005_0236a48c.inc.c"
};
extern u8 data_ov005_0236a5a0
    [33][4]; /* per entry: x and y in pixels, then grid column and row */
extern u8
    data_ov005_0236a5a1[33]
                       [4]; /* the same table from byte 1: [entry][0] is y */
extern const MenuButtonTemplate data_ov005_0236a500;

/* Per-frame update of the CO list screen (Ov005_SetUpCoListScreen runs
 * before it). Fades in, then waits in step 10:
 * - A (or its touch button) on another CO: saves the sub screen's palettes
 *   and starts a palette fade task, darkens both screens over 16 frames,
 *   loads that CO's wallpaper with Wallpaper_LoadBothOnLayer3, starts a fade back to the
 *   saved palettes and brightens again.
 * - R: hides the list (only BG3 and OBJ stay on) so Left and Right scroll
 *   the BG3 picture by up to 64 pixels; B brings the list back.
 * - B: fades out, hides both screens, saves the CO in gPlayRecords if it
 *   changed and resumes the task script.
 * Every frame plays a sound when the cursor moved, draws icon 0xd6 next to
 * the CO whose picture is shown, and applies the BG3 scroll on both
 * screens. */
void Ov005_UpdateCoListScreen(Ov005Task* task) {
  Unk_ov005_0236bba0* work = task->work;
  int fade;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Unk_027c0280*
      subSaved0280; /* needed to match: own variables for the sub screen in step 21 */
  int evy;
  Unk_02080440* subSaved027c;

  switch (task->step) {
    case 0:
      func_020048f4(32);
      MenuButtons_DisableTouch();
      task->step++;
    case 1:
      if (!Screens_IsTransitionRunning()) {
        task->step++;
      }
      break;
    case 2:
      MenuButtons_EnableTouch();
      Ov005_ClearCoListTouchFlags(task);
      task->step = 10;
      break;
    case 10:
      if ((gKeypad->pressed & PAD_BUTTON_A) || work->unk_10) {
        if (work->selectedEntry == work->unk_844) {
          work->unk_10 = 0;
        } else {
          Sound_PlaySe(0x10006);
          task->step = 20;
        }
      } else if ((gKeypad->pressed & PAD_BUTTON_R) || work->unk_08) {
        Sound_PlaySe(0x10006);
        task->step = 30;
      } else if ((gKeypad->pressed & PAD_BUTTON_B) || work->unk_0c) {
        Sound_PlaySe(0x10009);
        task->step = 100;
      }
      break;
    case 20:
      Ov005_DisableCoListTouch(task);
      task->frame = 0;
      task->frames = 16;
      work->unk_844 = work->selectedEntry;
      work->fadeTask = PaletteFade_StartToBlackScreen1(task, 16, 1, 1);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      Mem_Copy32(data_027c0280->paletteBuffer, work->savedPalettes, 0x400);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      task->step++;
      break;
    case 21:
      fade = Math_Ease(0, 0, 0x10000, task->frame, task->frames);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      evy = fade >> 12;
      data_027c0280->blendEvy = evy;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      subSaved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      subSaved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      data_027c0280->blendEvy = evy;
      Screen_SetCurrent(subSaved027c);
      Screen_SetCurrentGfx(subSaved0280);
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step++;
      }
      break;
    case 22:
      Wallpaper_LoadBothOnLayer3(data_ov005_0236a48c[work->unk_844]);
      task->frame = 0;
      task->frames = 16;
      func_020fd618(work->fadeTask);
      work->fadeTask = PaletteFade_StartFromBlackScreen1(
          task, &work->savedPalettes[0x20], 16, 1, 1);
      task->step++;
      break;
    case 23:
      fade = Math_Ease(0, 0x10000, 0, task->frame, task->frames);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      evy = fade >> 12;
      data_027c0280->blendEvy = evy;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->blendMode = BLEND_MODE_DARKEN;
      data_027c0280->blendEva = 0;
      data_027c0280->blendEvb = 0;
      data_027c0280->blendEvy = evy;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step++;
      }
      break;
    case 24:
      func_020fd618(work->fadeTask);
      Ov005_ClearCoListTouchFlags(task);
      Ov005_EnableCoListTouch(task);
      task->step = 10;
      break;
    case 30:
      Ov005_ClearCoListTouchFlags(task);
      MenuButtons_RemoveSelectable();
      MenuButton_Remove(240);
      MenuButton_Remove(241);
      data_027c0280->dispCnt &= ~0x100;
      data_027c0280->dispCnt &= ~0x200;
      data_027c0280->dispCnt &= ~0x400;
      data_027c0280->dispCnt |= 0x800;
      data_027c0280->dispCnt |= 0x1000;
      work->unk_84c = 0;
      work->button243Added = 0;
      work->button244Added = 0;
      work->unk_82c = 0;
      task->step++;
    case 31:
      if (gKeypad->held & PAD_KEY_LEFT) {
        work->unk_82c = -0x1000;
      } else if (gKeypad->held & PAD_KEY_RIGHT) {
        work->unk_82c = 0x1000;
      }
      if ((gKeypad->pressed & PAD_BUTTON_B) || work->unk_0c) {
        Sound_PlaySe(0x10009);
        task->step++;
        break;
      }
      work->scrollX += work->unk_82c;
      work->unk_82c = 0;
      if (work->scrollX < 0) {
        work->scrollX = 0;
      }
      if (work->scrollX > 0x40000) {
        work->scrollX = 0x40000;
      }
      Ov005_UpdateScrollButtons(task, work->scrollX >> 12);
      break;
    case 32:
      data_027c0280->dispCnt |= 0x100;
      data_027c0280->dispCnt |= 0x200;
      data_027c0280->dispCnt &= ~0x400;
      data_027c0280->dispCnt |= 0x800;
      data_027c0280->dispCnt |= 0x1000;
      Ov005_ClearCoListTouchFlags(task);
      Ov005_AddCoListButtons(task);
      MenuButton_Add(0, 0, &data_ov005_0236a500, 241);
      Ov005_RemoveScrollButtons(task);
      work->unk_84c = 1;
      task->step = 10;
      break;
    case 100:
      func_0200493c(32);
      MenuButtons_DisableTouch();
      task->step++;
    case 101:
      if (!Screens_IsTransitionRunning()) {
        task->step++;
      }
      break;
    case 102:
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
      Screen1_DestroyTextTasks();
      MenuButtons_RemoveSelectable();
      MenuButtons_RemoveUnselectable();
      Screen1_DestroyTextTasks();
      HelpBand_Stop();
      task->step++;
      break;
    case 103:
      if (work->unk_844 != work->unk_848) {
        Ov005_SaveCoListEntry(work->unk_844);
        EventSequenceTask_Start(task);
      }
      Task_ResumeScript(task);
      return;
  }

  if (work->selectedEntry != work->unk_840) {
    Sound_PlaySe(0x10002);
  }
  work->unk_840 = work->selectedEntry;

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  if (work->unk_84c) {
    Icon_Draw(0xd6, data_ov005_0236a5a0[work->unk_844][0] + 48,
              data_ov005_0236a5a1[work->unk_844][0], 0, 0);
  }
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = work->scrollY >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = work->scrollX >> 12;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = work->scrollY >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = work->scrollX >> 12;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
