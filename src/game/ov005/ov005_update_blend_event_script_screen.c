#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_023559dc.h"
#include "game/brightness_fade.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void* EventScript_Start(const void* script);
void Task_ResumeScript(Ov005Task* task);
int func_ov005_02355224(void);
void func_ov005_02355250(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
void func_ov005_02355238(void);

static TaskScriptCmd data_ov005_023683c8[36] = {
#include "data/ov005/data_ov005_023683c8.inc.c"
};
TaskScriptCmd data_ov005_023684e8[64] = {
#include "data/ov005/data_ov005_023684e8.inc.c"
};
TaskScriptCmd data_ov005_023686e8[100] = {
#include "data/ov005/data_ov005_023686e8.inc.c"
};

/* Event script screen like Ov005_UpdateEventScriptScreen. It fades both screens
 * in (two fade tasks), waits 32 frames, then alpha-blends the sub screen over
 * 64 frames (first target weight 0 to 16, second 16 to 0) before the script. */
void Ov005_UpdateBlendEventScriptScreen(Ov005Task* task) {
  Unk_ov005_023559dc* work = task->work;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int alpha;

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
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      alpha = Math_Ease(0, 0, 16, task->frame, task->frames);
      data_027c0280->blendMode = 1;
      data_027c0280->blendEva = alpha;
      data_027c0280->blendEvb = 16 - alpha;
      data_027c0280->blendEvy = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step = 10;
      }
      return;
    case 10:
      EventScript_Start(data_ov005_023683c8);
      func_ov005_02355250();
      task->step++;
      return;
    case 11:
      if (func_ov005_02355224() == 0) {
        task->step = 100;
      }
      return;
    case 100:
      work->fadeTask = BrightnessFade_StartOutBlackScreen0(task, 64);
      task->step++;
      return;
    case 101:
      if (func_020fd624(work->fadeTask)) {
        return;
      }
      func_020fd618(work->fadeTask);
      Task_ResumeScript(task);
      return;
  }
}
