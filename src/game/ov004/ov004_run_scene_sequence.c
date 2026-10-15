#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/campaign_map_music.h"
#include "game/menu_button.h"
#include "game/song.h"

void Ov004_StartControlTask(Unk_ov004_022bf158* state,
                            const TaskScriptCmd* script);
void Ov004_DestroyControlTask(Unk_ov004_022bf158* state);
void Ov004_DestroyAllSlotTasks(Unk_ov004_022bf158* state);
BOOL Ov004_SkipOnInput(Unk_ov004_022bf158* state, Ov004Task* task);
void Ov004_StartClock(Unk_ov004_022bf158* state);
void Ov004_ResetClock(Unk_ov004_022bf158* state);
void func_020fcb08(int a);
void Task_ResumeScript(void* task);
void Ov004_DestroySlotTasks(Unk_ov004_022bf158* state);
void Ov004_ApplyBg3Affine(Unk_ov004_022bf158* state);
void func_ov004_022bb388(Unk_ov004_022bf158* state);

extern const TaskScriptCmd data_ov004_022be3b8[];
extern const TaskScriptCmd data_ov004_022be3d8[];
extern const TaskScriptCmd data_ov004_022bdf50[];
extern const TaskScriptCmd data_ov004_022bdf68[];
extern const TaskScriptCmd data_ov004_022bded8[];
extern const TaskScriptCmd data_ov004_022bdf08[];
extern const TaskScriptCmd data_ov004_022bdf20[];
extern const TaskScriptCmd data_ov004_022bdf38[];
extern const TaskScriptCmd data_ov004_022bdef0[];
extern const TaskScriptCmd data_ov004_022bdf80[];

/* Ends a scene once its control task has finished: wait (or skip), then
 * clear the scene and go to nextStep. */
#define SCENE_WAIT(nextStep)            \
  if (Ov004_SkipOnInput(state, task)) { \
    break;                              \
  }                                     \
  if (state->controlRunning != 0) {     \
    break;                              \
  }                                     \
  Ov004_ResetClock(state);              \
  Ov004_DestroyControlTask(state);      \
  Ov004_DestroyAllSlotTasks(state);     \
  task->step = (nextStep)

/* Script step: plays ten scenes, each a control task with its own script.
 * The first two cannot be skipped; from the third on, a touch, A or Start
 * jumps to step 1000. Step 1000 turns both screens white and clears the
 * scene, then the next frame resumes the script. */
void Ov004_RunSceneSequence(Ov004Task* task) {
  Unk_ov004_022bf158* state = task->state;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;

  switch ((u32)task->step) { /* needed to match: unsigned compares */
    case 10:
      Ov004_StartControlTask(state, data_ov004_022be3b8);
      task->step++;
    case 11:
      if (state->controlRunning != 0) {
        break;
      }
      Ov004_DestroyControlTask(state);
      Ov004_DestroyAllSlotTasks(state);
      task->step++;
    case 12:
      Ov004_StartControlTask(state, data_ov004_022be3d8);
      task->step++;
    case 13:
      if (Ov004_SkipOnInput(state, task)) {
        break;
      }
      if (state->controlRunning != 0) {
        break;
      }
      Ov004_DestroyControlTask(state);
      Ov004_DestroyAllSlotTasks(state);
      Sound_StartSong(41);
      Ov004_StartClock(state);
      task->step = 20;
      break;
    case 20:
      Ov004_StartControlTask(state, data_ov004_022bdf50);
      task->step++;
    case 21:
      SCENE_WAIT(30);
    case 30:
      Ov004_StartControlTask(state, data_ov004_022bdf68);
      task->step++;
    case 31:
      SCENE_WAIT(40);
    case 40:
      Ov004_StartControlTask(state, data_ov004_022bded8);
      task->step++;
    case 41:
      SCENE_WAIT(50);
    case 50:
      Ov004_StartControlTask(state, data_ov004_022bdf08);
      task->step++;
    case 51:
      SCENE_WAIT(60);
    case 60:
      Ov004_StartControlTask(state, data_ov004_022bdf20);
      task->step++;
    case 61:
      SCENE_WAIT(70);
    case 70:
      Ov004_StartControlTask(state, data_ov004_022bdf38);
      task->step++;
    case 71:
      SCENE_WAIT(80);
    case 80:
      Ov004_StartControlTask(state, data_ov004_022bdef0);
      task->step++;
    case 81:
      SCENE_WAIT(90);
    case 90:
      Ov004_StartControlTask(state, data_ov004_022bdf80);
      task->step++;
    case 91:
      SCENE_WAIT(1000);
    case 1000:
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      data_027c0280->masterBrightness = 16;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedScreen = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->masterBrightness = 16;
      Screen_SetCurrent(savedScreen);
      Screen_SetCurrentGfx(savedGfx);
      Ov004_DestroyControlTask(state);
      Ov004_DestroyAllSlotTasks(state);
      Ov004_DestroySlotTasks(state);
      MenuButton_Remove(253);
      func_020fcb08(0);
      task->step++;
      break;
    case 1001:
      Task_ResumeScript(task);
      break;
  }
  Ov004_ApplyBg3Affine(state);
  func_ov004_022bb388(state);
}
