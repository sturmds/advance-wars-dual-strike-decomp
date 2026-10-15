#include "types.h"
#include "sdk/gx.h"
#include "game/screen_other_map.h"
#include "game/task.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/brightness_fade.h"
#include "game/screen.h"
#include "game/math.h"
#include "game/campaign_map_music.h"
#include "game/song.h"

BOOL func_020fd624(Task* task);
void func_020fd618(Task* task);
BOOL Touch_IsPressed(void);
BOOL Keypad_ConfirmPressed(void);
void Screens_BlackOut(void);
void func_0200493c(u16 value);
void Sound_PlaySe(u32 soundId);
void Task_ResumeScript(Ov004Task* task);
void Ov004_Init3d(void);
void Ov004_SendTexturedQuad256(int x, int y, int z, int sSize, int tSize, int w,
                               int h, int s0, int t0, u32 texAddr, u32 palAddr,
                               int alpha, int polyId);
int func_ov004_022ada10(void);
void Ov004_CreateChildTasks(Unk_ov004_022bf158* state);
void Ov004_QueueBottomSprites(Unk_ov004_022bf158* state);
void Ov004_DestroyChildTasks(Unk_ov004_022bf158* state);
void Ov004_DestroySceneTasks(Unk_ov004_022bf158* state);

extern TaskScriptCmd data_ov004_022beff4[];

#define SET_LAYERS_SHOWN(b0, b1, b2, b3, o) \
  do {                                      \
    GFX_DISPCNT(data_027c0280)->bg0 = (b0); \
    GFX_DISPCNT(data_027c0280)->bg1 = (b1); \
    GFX_DISPCNT(data_027c0280)->bg2 = (b2); \
    GFX_DISPCNT(data_027c0280)->bg3 = (b3); \
    GFX_DISPCNT(data_027c0280)->obj = (o);  \
  } while (0)

/* Script step for a screen that waits for a press. It runs two 64-frame
 * screen fades (func_020fcd28, BrightnessFade_StartInWhiteScreen1), then brings in
 * two 256 by 128 textured quads on the 3D screen: unk_8f8 and
 * unk_8fc are their alphas (0-31), unk_900 and unk_904 their depths and
 * unk_908 and unk_90c their palettes (in 0x200-byte steps). The first quad
 * fades in over 60 frames while both move to depth 0, with sound 84 at frame
 * 23 and a white flash at the end. Then it shows BG0, BG2 and OBJ on the
 * other screen, starts song 66 (or the task data_ov004_022beff4 when the
 * intro ran to its end: unk_008 is set by Ov004_SkipOnInput) and turns on
 * the two bottom sprites. A touch or a press of A or Start ends the step
 * with unk_000 = 1; once unk_1bf0 is set (func_ov004_022bd4d8) it fades out
 * and ends with unk_000 = 0. */
void Ov004Task_RunPressStartScreen(Ov004Task* task) {
  Unk_ov004_022bf158* state = task->state;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  switch ((u32)task->step) {
    case 0:
      state->unk_8f8 = 0;
      state->unk_8fc = 0;
      state->unk_900 = 0;
      state->unk_904 = 0;
      state->unk_908 = 0;
      state->unk_90c = 0;
      state->unk_810 = func_020fcd28(task, 64);
      state->unk_814 = BrightnessFade_StartInWhiteScreen1(task, 64);
      task->step++;
    case 1:
      if (func_020fd624(state->unk_810)) {
        break;
      }
      if (func_020fd624(state->unk_814)) {
        break;
      }
      func_020fd618(state->unk_810);
      func_020fd618(state->unk_814);
      task->step = 10;
      break;
    case 10:
      state->unk_8f8 = 0;
      state->unk_8fc = 30;
      state->unk_900 = 0;
      state->unk_904 = 0;
      state->unk_908 = 1;
      state->unk_90c = 1;
      task->frame = 0;
      task->duration = 60;
      task->step++;
    case 11:
      state->unk_904 = Math_Ease(0, 1000, 0, task->frame, task->duration);
      state->unk_900 = Math_Ease(0, -200, 0, task->frame, task->duration);
      state->unk_8f8 = Math_Ease(2, 0, 31, task->frame, task->duration);
      if (task->frame == 23) {
        Sound_PlaySe(84);
      }
      if (task->frame == task->duration) {
        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        data_027c0280->masterBrightness = 16;
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);

        savedGfx = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_04);
        savedState = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_04);
        data_027c0280->masterBrightness = 16;
        Screen_SetCurrent(savedState);
        Screen_SetCurrentGfx(savedGfx);
      }
      if (task->frame < task->duration) {
        task->frame++;
        break;
      }
      state->unk_908 = 0;
      state->unk_90c = 0;
      task->step = 20;
      break;
    case 20:
      state->unk_8f8 = 0;
      state->unk_8fc = 31;
      state->unk_900 = 0;
      state->unk_904 = 0;
      state->unk_908 = 0;
      state->unk_90c = 0;
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      SET_LAYERS_SHOWN(1, 0, 1, 0, 1);
      Screen_SetCurrent(savedState);
      Screen_SetCurrentGfx(savedGfx);
      state->unk_1bfc = func_020fcd28(TASK_LIST(4), 24);
      state->unk_1c00 = BrightnessFade_StartInWhiteScreen1(TASK_LIST(4), 24);
      state->unk_1c0c = 1;
      if (state->unk_008 == 0) {
        state->unk_1bf8 = Task_Create(data_ov004_022beff4, TASK_LIST(4));
      } else {
        Sound_StartSong(66);
      }
      Ov004_CreateChildTasks(state);
      task->step = 30;
    case 30:
      if (func_020fd624(state->unk_1bfc)) {
        break;
      }
      if (!func_020fd624(state->unk_1c00)) {
        task->step = 50;
      }
      break;
    case 50:
      task->step++;
      break;
    case 51:
      if (Touch_IsPressed() || Keypad_ConfirmPressed()) {
        Sound_PlaySe(0x10006);
        task->step = 100;
      } else if (state->unk_1bf0 != 0) {
        task->step = 1000;
      }
      break;
    case 100:
      Sound_FadeOutSongOver(1);
      Screen_SetBothNibbles(0);
      func_0200493c(32);
      task->step++;
    case 101:
      if (Screens_IsTransitionRunning()) {
        break;
      }
      Screens_BlackOut();
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      SET_LAYERS_SHOWN(0, 0, 0, 0, 0);
      Screen_SetCurrent(savedState);
      Screen_SetCurrentGfx(savedGfx);
      savedGfx = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      savedState = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      SET_LAYERS_SHOWN(0, 0, 0, 0, 0);
      Screen_SetCurrent(savedState);
      Screen_SetCurrentGfx(savedGfx);
      state->unk_000 = 1;
      Ov004_DestroyChildTasks(state);
      Ov004_DestroySceneTasks(state);
      Task_ResumeScript(task);
      break;
    case 1000:
      Sound_FadeOutSongOver(1);
      state->unk_810 = BrightnessFade_StartOutBlackScreen0(task, 64);
      state->unk_814 = func_020fcd50(task, 64);
      task->step++;
    case 1001:
      if (func_020fd624(state->unk_810)) {
        break;
      }
      if (func_020fd624(state->unk_814)) {
        break;
      }
      func_020fd618(state->unk_810);
      func_020fd618(state->unk_814);
      state->unk_000 = 0;
      Ov004_DestroyChildTasks(state);
      Ov004_DestroySceneTasks(state);
      Task_ResumeScript(task);
      break;
  }

  Ov004_Init3d();
  *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3000) | 8;
  if (state->unk_8fc != 0) {
    Ov004_SendTexturedQuad256(-128, -48, state->unk_904, 5, 4, 256, 128, 0, 0,
                              0, state->unk_90c << 9, state->unk_8fc, 0);
  }
  if (state->unk_8f8 != 0) {
    Ov004_SendTexturedQuad256(-128, -48, state->unk_900, 5, 4, 256, 128, 0, 0,
                              0, state->unk_908 << 9, state->unk_8f8, 1);
  }
  func_ov004_022ada10();
  Ov004_QueueBottomSprites(state);
}
