#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_023559dc.h"
#include "game/brightness_fade.h"

BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void Screens_BlackOut(void);
Task* func_020fcb08(int arg);
BOOL BrightnessFade_IsActive(Task* task);
void func_020fccb0(Task* task);
void* EventScript_Start(const void* script);
void Task_ResumeScript(Ov005Task* task);
int func_ov005_02357830(void);
void func_ov005_0235785c(void);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
void func_ov005_02357844(void);

static TaskScriptCmd data_ov005_02368b60[40] = {
#include "data/ov005/data_ov005_02368b60.inc.c"
};

/* Per-frame update, like Ov005_UpdateEventScriptScreen with a longer ending:
 * fade in (64 frames), wait 32 frames, run event script data_ov005_02368b60
 * until func_ov005_02357830 reports it is done, then fade out with two fade
 * tasks, hide every layer and OBJ on both screens, fade out the song, and
 * resume the task script. */
void Ov005_UpdateEventScriptScreenEndSong(Ov005Task* task) {
  Unk_ov005_023559dc* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  switch (task->step) {
    case 0:
      work->fadeTask = func_020fcdc8(task, 64);
      task->step++;
    case 1:
      if (func_020fd624(work->fadeTask)) {
        return;
      }
      func_020fd618(work->fadeTask);
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
        task->step = 10;
      }
      return;
    case 10:
      EventScript_Start(data_ov005_02368b60);
      func_ov005_0235785c();
      task->step++;
      return;
    case 11:
      if (func_ov005_02357830() == 0) {
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
      task->step++;
      return;
    case 102:
      work->songFadeTask = func_020fcb08(1);
      task->step++;
    case 103:
      if (BrightnessFade_IsActive(work->songFadeTask)) {
        return;
      }
      func_020fccb0(work->songFadeTask);
      Task_ResumeScript(task);
      return;
  }
}
