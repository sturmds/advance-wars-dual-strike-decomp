/* Built without optimization, like the rest of the name entry screen. */
#pragma optimization_level 0

#include "types.h"
#include "game/name_entry_work.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/keypad.h"

void func_02004984(u16 frames);
void func_020049cc(u16 frames);
int ScreenTransition_IsRunning(void);
void NameEntry_StartScript(const void* script);
int NameEntry_IsScriptRunning(void);
int func_020b983c(void);
void func_020b9864(void);
void func_020b9890(void);
void Sound_PlaySe(int seId);
void Task_ResumeScript(NameEntryTask* task);
void MenuButtons_RemoveSelectable(void);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Gfx_RequestLayerCopy(int layer);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
int NameEntry_GetControlKey(NameEntry* entry);
BOOL NameEntry_AppendChar(NameEntry* entry);
void NameEntry_AddKeyButtons(NameEntry* entry);
void NameEntry_ClearText(NameEntry* entry);
void NameEntry_DeleteChar(NameEntry* entry);
void NameEntry_DrawText(NameEntry* entry);
void NameEntry_PlaceButtonSprites(NameEntry* entry);
void NameEntry_PlaceCaret(NameEntry* entry);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern u16 gEventChoiceAnswer;
extern u8 data_0216ab54[];
extern u8 data_0216ab84[];
extern u8 data_0216abb4[];
extern u8 data_0216abe4[];
extern u8 data_0216ac44[];
extern u8 gNameEntryStartText[];
extern u8 gNameEntryText[];

/* Per-frame update of the name entry task. Steps 0-1 start a 32-frame task
 * (func_02004984; probably a fade) and wait for it; for the 8-character
 * field steps 2-7 then run three message scripts. Step 11 handles keys: A (or
 * a touched key) types a character or confirms a control key, B deletes a
 * character, START or control key 1 finishes. Steps 90-95 ask a question
 * by script and pick the next step from the answer (gEventChoiceAnswer, 0: yes).
 * Steps 100-103 run func_020049cc the same way and resume the task's script, restoring the
 * starting text when func_020b983c returns 0. task->maxLength is only read
 * at setup; here it holds a flag, then the step to go to after step 95. */
void NameEntry_Update(NameEntryTask* task) {
  NameEntry* work = task->work;
  int key;

  /* needed to match: the step compares are unsigned */
  switch ((u32)task->step) {
    case 0:
      func_02004984(32);
      task->step++;
    case 1:
      if (ScreenTransition_IsRunning() == 0) {
        if (work->maxLength == 16) {
          task->step = 10;
        } else {
          task->step++;
        }
      }
      break;
    case 2:
      NameEntry_StartScript(data_0216ac44);
      task->step++;
    case 3:
      if (NameEntry_IsScriptRunning() == 0) {
        task->step++;
      }
      break;
    case 4:
      NameEntry_StartScript(data_0216ab84);
      task->step++;
    case 5:
      if (NameEntry_IsScriptRunning() == 0) {
        task->step++;
      }
      break;
    case 6:
      NameEntry_StartScript(data_0216ab54);
      task->step++;
    case 7:
      if (NameEntry_IsScriptRunning() == 0) {
        task->step = 10;
      }
      break;
    case 10:
      work->unk_11c = 0;
      work->unk_120 = 0;
      work->unk_124 = 0;
      work->unk_128 = 0;
      NameEntry_AddKeyButtons(work);
      task->step++;
    case 11:
      if ((gKeypad->pressed & PAD_BUTTON_A) || work->unk_11c == 1) {
        key = NameEntry_GetControlKey(work);
        if (key == -1) {
          work->unk_11c = 0;
          if (NameEntry_AppendChar(work)) {
            Sound_PlaySe(0x10006);
          }
          break;
        }
        switch (key) {
          case 0:
            work->unk_124 = 1;
            break;
          case 1:
            work->unk_128 = 1;
            break;
          case 2:
            work->unk_120 = 1;
            break;
        }
      }
      if ((gKeypad->pressed & PAD_BUTTON_B) || work->unk_120 == 1) {
        Sound_PlaySe(0x10009);
        work->unk_120 = 0;
        NameEntry_ClearText(work);
        NameEntry_DeleteChar(work);
        break;
      }
      if (work->unk_124 == 1) {
        work->unk_124 = 0;
        MenuButtons_RemoveSelectable();
        task->maxLength = 0;
        task->step = 90;
        break;
      }
      if ((gKeypad->pressed & PAD_BUTTON_START) || work->unk_128 == 1) {
        MenuButtons_RemoveSelectable();
        if (work->maxLength == 16) {
          func_020b9890();
          task->step = 100;
        } else {
          task->maxLength = 1;
          task->step = 90;
        }
      }
      break;
    case 90:
      NameEntry_StartScript(data_0216ac44);
      task->step++;
    case 91:
      if (NameEntry_IsScriptRunning() == 0) {
        task->step++;
      }
      break;
    case 92:
      if (task->maxLength != 0) {
        NameEntry_StartScript(data_0216abb4);
      } else {
        NameEntry_StartScript(data_0216abe4);
      }
      task->step++;
    case 93:
      if (NameEntry_IsScriptRunning() == 0) {
        if (gEventChoiceAnswer == 0) {
          if (task->maxLength != 0) {
            func_020b9890();
          } else {
            func_020b9864();
          }
          task->maxLength = 100;
        } else {
          task->maxLength = 10;
        }
        task->step++;
      }
      break;
    case 94:
      NameEntry_StartScript(data_0216ab54);
      task->step++;
    case 95:
      if (NameEntry_IsScriptRunning() == 0) {
        task->step = task->maxLength;
      }
      break;
    case 100:
      func_020049cc(32);
      task->step++;
    case 101:
      if (ScreenTransition_IsRunning() == 0) {
        task->step++;
      }
      break;
    case 102:
      task->step++;
      break;
    case 103:
      if (func_020b983c() == 0) {
        Mem_Copy32(gNameEntryStartText, gNameEntryText, NAME_ENTRY_TEXT_SIZE);
      }
      Task_ResumeScript(task);
      return;
  }

  if (work->cursor != work->prevCursor) {
    Sound_PlaySe(0x10002);
  }
  work->prevCursor = work->cursor;
  NameEntry_PlaceButtonSprites(work);
  {
    Unk_027c0280* savedGfx = data_027c0280;
    Unk_02080440* savedScreen;

    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreen = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    NameEntry_DrawText(work);
    Gfx_RequestLayerCopy(0);
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);
  }
  NameEntry_PlaceCaret(work);
}
