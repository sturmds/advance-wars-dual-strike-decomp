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
void Sound_StartFadeOutTask(int holdScript, int fadeLength);
BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void* EventScript_Start(const void* script);
void Task_ResumeScript(Ov005Task* task);
int func_ov005_02355224(void);
void func_ov005_02355250(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern const TaskScriptCmd data_ov005_023686e8[];

/* Event script screen like Ov005_UpdateEventScriptScreen. Each frame BG layers
 * 2 and 3 of the main screen scroll horizontally by a linear tween
 * (0 to 256 and 256 to 512, over scrollFrames). The fade out starts a song
 * fade out too. */
void Ov005_UpdateScrollingEventScriptScreen(Ov005Task* task) {
  Unk_ov005_023559dc* work = task->work;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  int scrollX2;
  int scrollX3;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  scrollX2 = Math_Ease(0, 0, 0x100000, work->scrollFrame, work->scrollFrames);
  scrollX3 =
      Math_Ease(0, 0x100000, 0x200000, work->scrollFrame, work->scrollFrames);
  if (work->scrollFrame < work->scrollFrames) {
    work->scrollFrame++;
  }
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = scrollX2 >> 12;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = scrollX3 >> 12;
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

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
      task->frames = 64;
      task->step++;
    case 3:
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step = 10;
      }
      return;
    case 10:
      EventScript_Start(data_ov005_023686e8);
      func_ov005_02355250();
      task->step++;
      return;
    case 11:
      if (func_ov005_02355224() == 0) {
        task->step = 100;
      }
      return;
    case 100:
      Sound_StartFadeOutTask(0, 4);
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
