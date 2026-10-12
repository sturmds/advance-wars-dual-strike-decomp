#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb90.h"
#include "game/brightness_fade.h"

BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void Screens_BlackOut(void);
void Task_ResumeScript(Ov005Task* task);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

/* Per-frame update of the large-work screen (task script data_ov005_02369634):
 * fade both screens in (64 frames), show the screen for about 300 frames, fade
 * both out, hide every layer and OBJ on both screens and resume the task
 * script. */
void Ov005_UpdateTimedScreen(Ov005Task* task) {
  Unk_ov005_0236bb90* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

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
      task->step = 10;
      return;
    case 10:
      task->frame = 0;
      task->frames = 300;
      task->step++;
    case 11:
      if (task->frame <= task->frames) {
        task->frame++;
      }
      if (task->frame > task->frames) {
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
      Task_ResumeScript(task);
      return;
  }
}
