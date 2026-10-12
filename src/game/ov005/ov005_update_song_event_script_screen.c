#include "types.h"
#include "game/task.h"
#include "game/song.h"
#include "game/ov005_task.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bb84.h"
#include "game/brightness_fade.h"

void Sound_FadeMusicVolume(int from, int to, int frames);
BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
void Screens_BlackOut(void);
Task* func_020fcb08(int arg);
BOOL BrightnessFade_IsActive(Task* task);
void func_020fccb0(Task* task);
void* EventScript_Start(const void* script);
void Task_ResumeScript(Ov005Task* task);
int func_ov005_02355224(void);
void func_ov005_02355250(void);

extern const TaskScriptCmd data_ov005_023684e8[];

/* Per-frame update: start song 55 and raise its volume, fade in (64 frames),
 * wait 32 frames, run the event script data_ov005_023684e8 until
 * func_ov005_02355224 reports it is done, fade both screens out and hide
 * their layers, fade the song out, then resume the task script. */
void Ov005_UpdateSongEventScriptScreen(Ov005Task* task) {
  Unk_ov005_0236bb84* work = task->work;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  switch (task->step) {
    case 0:
      Sound_StartSong(55);
      Sound_FadeMusicVolume(0, 127, 64);
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
      task->step++;
    case 11:
      EventScript_Start(data_ov005_023684e8);
      func_ov005_02355250();
      task->step++;
      return;
    case 12:
      if (func_ov005_02355224() == 0) {
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
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      GFX_DISPCNT(data_027c0280)->bg0 = 0;
      GFX_DISPCNT(data_027c0280)->bg1 = 0;
      GFX_DISPCNT(data_027c0280)->bg2 = 0;
      GFX_DISPCNT(data_027c0280)->bg3 = 0;
      GFX_DISPCNT(data_027c0280)->obj = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      GFX_DISPCNT(data_027c0280)->bg0 = 0;
      GFX_DISPCNT(data_027c0280)->bg1 = 0;
      GFX_DISPCNT(data_027c0280)->bg2 = 0;
      GFX_DISPCNT(data_027c0280)->bg3 = 0;
      GFX_DISPCNT(data_027c0280)->obj = 0;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
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
