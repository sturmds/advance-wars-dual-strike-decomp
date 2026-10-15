#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Screen_SetCurrentBoth(void* state);
void* Task_FindByScript(const void* script);
void Task_ResumeScript(Task* task);
void func_02050ca8(void* parentOrList);
void TitleCall_SetBg3Zoom(int scale, int angle);

extern const TaskScriptCmd data_0215bc10[];

#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Task script step, 120 frames on both screens. BG3 zooms in (TitleCall_SetBg3Zoom
 * eases its arguments from 192 and 384 to 256). Frames 1-31: the screens
 * brighten out of darken mode (evy 16 down by half a step per frame). From
 * frame 32: layer 3 is alpha-blended over layers 1 and 2 while EVA eases from
 * 16 to 0, so layer 3 fades out. Frame 32 also starts the func_02050ca8 task
 * under the task running data_0215bc10. The script resumes when EVA is 0. */
void Screens_ZoomInFadeUpdate(Task* task) {
  int eva;

  FRAME(task)++;
  TitleCall_SetBg3Zoom(Math_Ease(4, 192, 256, FRAME(task), 120),
                       Math_Ease(4, 384, 256, FRAME(task), 120));

  if (FRAME(task) < 32) {
    Unk_027c0280* saved0280;
    Unk_02080440* saved027c;

    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c0280->blendMode = BLEND_MODE_DARKEN;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 16 - (FRAME(task) >> 1);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c0280->blendMode = BLEND_MODE_DARKEN;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 16 - (FRAME(task) >> 1);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  } else {
    /* Needed to match: at function scope the saved pointers swap registers. */
    Unk_027c0280* saved0280;
    Unk_02080440* saved027c;

    eva = Math_Ease(4, 16, 0, FRAME(task) - 32, 88);
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = eva;
    data_027c0280->blendEvb = 16 - eva;
    data_027c0280->blendEvy = 0;
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = eva;
    data_027c0280->blendEvb = 16 - eva;
    data_027c0280->blendEvy = 0;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    if (eva == 0) {
      Task_ResumeScript(task);
      return;
    }
  }

  if (FRAME(task) == 32) {
    Unk_027c0280* saved0280;
    Unk_02080440* saved027c;

    func_02050ca8(Task_FindByScript(data_0215bc10));
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 0;
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 0;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
}
