#include "types.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bbac.h"
#include "game/screen.h"

void Sound_PlaySe(u32 soundId);
void MenuButtons_RemoveUnselectable(void);
void Wallpaper_DestroyTasks(void);
void Task_ResumeScript(Ov005Task* task);
void func_020048f4(u16 frames);
void func_0200493c(u16 frames);
void Screen1_DestroyTextTasks(void);
BOOL Ov005SoundRoom_CellChanged(void);
int Ov005SoundRoom_IsMusicChanging(void);
void Ov005SoundRoom_ClearMusicRequest(void);
void Ov005SoundRoom_SaveCell(void);
void Ov005SoundRoom_SetSlideDirection(Unk_ov005_0236bbac* work);
void func_ov005_02364418(Unk_ov005_0236bbac* work);
void func_ov005_02364708(Unk_ov005_0236bbac* work);
void func_ov005_02364a1c(Unk_ov005_0236bbac* work);
void Ov005SoundRoom_DrawPageNumber(Unk_ov005_0236bbac* work);
void Ov005_SoundRoom_AddCellButtons(Unk_ov005_0236bbac* work);
void Ov005_UpdateSoundRoomSprites(Unk_ov005_0236bbac* work);
void Ov005SoundRoom_PickCellCo(Unk_ov005_0236bbac* work);
void Ov005SoundRoom_DrawSelectedCellName(Unk_ov005_0236bbac* work);
void Ov005SoundRoom_DrawCellNames(Unk_ov005_0236bbac* work);

/* Per-frame update of the Sound Room. Fades in, opens window 0 on the sub
 * screen (x 40-216, y 168-184; BG0 hidden outside it), then handles input:
 * A plays the selected song unless it is already playing, X stops it
 * (Ov005SoundRoom_ClearMusicRequest clears the playing song), L/left and R/right turn the
 * page with wraparound, B leaves. Each input also has a touch button that
 * sets the matching *Touched flag. On B it waits for
 * Ov005SoundRoom_IsMusicChanging to clear, fades out, frees the screen's tasks and
 * resumes the task script. Every frame ends by redrawing the list. */
void Ov005_UpdateSoundRoom(Ov005Task* task) {
  Unk_ov005_0236bbac* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  switch (task->step) {
    case 0:
      func_020048f4(32);
      task->step++;
      break;
    case 1:
      if (Screens_IsTransitionRunning()) {
        break;
      }
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->dispCnt |= 0x2000;
      data_027c0280->dispCnt &= ~0x4000;
      data_027c0280->dispCnt &= ~0x8000;
      data_027c0280->win0In.bg[data_027c0280->layer0Bg] = 1;
      data_027c0280->win0In.bg[data_027c0280->layer1Bg] = 1;
      data_027c0280->win0In.bg[data_027c0280->layer2Bg] = 1;
      data_027c0280->win0In.bg[data_027c0280->layer3Bg] = 1;
      data_027c0280->win0In.obj = 1;
      data_027c0280->winOut.bg[data_027c0280->layer0Bg] = 0;
      data_027c0280->winOut.bg[data_027c0280->layer1Bg] = 1;
      data_027c0280->winOut.bg[data_027c0280->layer2Bg] = 1;
      data_027c0280->winOut.bg[data_027c0280->layer3Bg] = 1;
      data_027c0280->winOut.obj = 1;
      data_027c0280->win0Left = 40;
      data_027c0280->win0Top = 168;
      data_027c0280->win0Right = 216;
      data_027c0280->win0Bottom = 184;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      task->step = 10;
      break;
    case 10:
      if ((gKeypad->pressed & PAD_BUTTON_A) || work->aTouched == 1) {
        if (Ov005SoundRoom_CellChanged() && !Ov005SoundRoom_IsMusicChanging()) {
          Sound_PlaySe(0x10006);
          Ov005SoundRoom_SaveCell();
        }
        work->aTouched = 0;
      } else if ((gKeypad->pressed & PAD_BUTTON_B) || work->bTouched == 1) {
        Sound_PlaySe(0x10009);
        MenuButtons_RemoveSelectable();
        MenuButtons_DisableTouch();
        work->bTouched = 0;
        Ov005SoundRoom_ClearMusicRequest();
        task->step = 100;
      } else if ((gKeypad->pressed & PAD_BUTTON_X) || work->xTouched == 1) {
        work->xTouched = 0;
        Ov005SoundRoom_ClearMusicRequest();
      } else if ((gKeypad->pressed & PAD_BUTTON_L) ||
                 (gKeypad->repeat & PAD_KEY_LEFT) || work->lTouched == 1) {
        work->lTouched = 0;
        if (work->page <= 0) {
          work->page = work->lastPage;
        } else {
          work->page--;
        }
        work->selection = work->pageStartSelection;
        work->shownSelection = -1;
        MenuButtons_RemoveSelectable();
        Ov005_SoundRoom_AddCellButtons(work);
        Ov005SoundRoom_SetSlideDirection(work);
        Ov005SoundRoom_PickCellCo(work);
      } else if ((gKeypad->pressed & PAD_BUTTON_R) ||
                 (gKeypad->repeat & PAD_KEY_RIGHT) || work->rTouched == 1) {
        work->rTouched = 0;
        if (work->page >= work->lastPage) {
          work->page = 0;
        } else {
          work->page++;
        }
        work->selection = work->pageStartSelection;
        work->shownSelection = -1;
        MenuButtons_RemoveSelectable();
        Ov005_SoundRoom_AddCellButtons(work);
        Ov005SoundRoom_SetSlideDirection(work);
        Ov005SoundRoom_PickCellCo(work);
      }
      break;
    case 100:
      if (!Ov005SoundRoom_IsMusicChanging()) {
        task->step++;
      }
      break;
    case 101:
      task->step++;
    case 102:
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->dispCnt &= ~0x2000;
      data_027c0280->dispCnt &= ~0x4000;
      data_027c0280->dispCnt &= ~0x8000;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      func_0200493c(32);
      task->step++;
      break;
    case 103:
      if (!Screens_IsTransitionRunning()) {
        task->step++;
      }
      break;
    case 104:
      Wallpaper_DestroyTasks();
      Screen1_DestroyTextTasks();
      func_ov005_02364708(work);
      func_ov005_02364a1c(work);
      func_ov005_02364418(work);
      MenuButtons_RemoveUnselectable();
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
    case 105:
      Task_ResumeScript(task);
      return;
  }

  if (work->selection != work->shownSelection) {
    Sound_PlaySe(0x10002);
  }
  work->shownSelection = work->selection;
  Ov005_UpdateSoundRoomSprites(work);
  Ov005SoundRoom_DrawCellNames(work);
  Ov005SoundRoom_DrawSelectedCellName(work);
  Ov005SoundRoom_DrawPageNumber(work);
}
