#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Screen_SetCurrentBoth(void* state);
void Screens_SetBg3Zoom(int scale, int angle);
void Task_ResumeScript(Task* task);
void func_02032b28(void* parentOrList);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define GFX data_027c0280

/* Alpha blend with layer 3 on top. Below it: layer 2 on the screen of
 * data_027c0288.unk_00, layers 0-2 on the other. */
static inline void StartLayer3Blend(void) {
  GFX->blendMode = BLEND_MODE_ALPHA;
  GFX->blendEva = 16;
  GFX->blendEvb = 0;
  GFX->blendEvy = 0;
  GFX->blendTopBg[GFX->layer0Bg] = 0;
  GFX->blendTopBg[GFX->layer1Bg] = 0;
  GFX->blendTopBg[GFX->layer2Bg] = 0;
  GFX->blendTopBg[GFX->layer3Bg] = 1;
  GFX->blendTopObj = 0;
  if (data_027c027c == data_027c0288.unk_00) {
    GFX->blendBottomBg[GFX->layer0Bg] = 0;
    GFX->blendBottomBg[GFX->layer1Bg] = 0;
    GFX->blendBottomBg[GFX->layer2Bg] = 1;
    GFX->blendBottomBg[GFX->layer3Bg] = 0;
    GFX->blendBottomObj = 0;
  } else {
    GFX->blendBottomBg[GFX->layer0Bg] = 1;
    GFX->blendBottomBg[GFX->layer1Bg] = 1;
    GFX->blendBottomBg[GFX->layer2Bg] = 1;
    GFX->blendBottomBg[GFX->layer3Bg] = 0;
    GFX->blendBottomObj = 0;
  }
}

/* Layer priorities back to 0-3 and blending off. */
static inline void ResetLayersAndBlend(void) {
  GFX->bgCnt[GFX->layer0Bg].priority = 0;
  GFX->bgCnt[GFX->layer1Bg].priority = 1;
  GFX->bgCnt[GFX->layer2Bg].priority = 2;
  GFX->bgCnt[GFX->layer3Bg].priority = 3;
  GFX->blendMode = 0;
  GFX->blendEva = 16;
  GFX->blendEvb = 0;
  GFX->blendEvy = 0;
}

/* Sets layer 3's blend weight to eva / 16 on both screens, the layers
 * below to the rest. */
static inline void Screens_SetLayer3Alpha(int eva) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  GFX->blendEva = eva;
  GFX->blendEvb = 16 - eva;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  GFX->blendEva = eva;
  GFX->blendEvb = 16 - eva;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}

/* Task update, 180 frames on both screens: layer 2 scrolls from x 64 to
 * 0 and BG3 zooms (Screens_SetBg3Zoom 192 to 256, second argument 128 to
 * 256). Frames 0-32 ease blendEvy from 16 to 0; frame 32 switches to an
 * alpha blend that fades layer 3 out over the remaining 148 frames. Frame
 * 52 starts func_02032b28. Frame 180 resets priorities and blending,
 * clears the counter and resumes the task's script. */
void Screens_ZoomFadeUpdate(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int value;

  if (FRAME(task) < 180) {
    FRAME(task)++;
  }
  value = Math_Ease(4, 64, 0, FRAME(task), 180);
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  GFX->bgScrollX[GFX->layer2Bg] = value;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  GFX->bgScrollX[GFX->layer2Bg] = value;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  value = Math_Ease(4, 192, 256, FRAME(task), 180);
  Screens_SetBg3Zoom(value, Math_Ease(4, 128, 256, FRAME(task), 180));

  if (FRAME(task) <= 32) {
    value = Math_Ease(0, 16, 0, FRAME(task), 32);
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    GFX->blendEvy = value;
    if (FRAME(task) == 32) {
      StartLayer3Blend();
    }
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    GFX->blendEvy = value;
    if (FRAME(task) == 32) {
      StartLayer3Blend();
    }
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  } else {
    Screens_SetLayer3Alpha(Math_Ease(4, 16, 0, FRAME(task) - 32, 148));
  }

  if (FRAME(task) == 52) {
    func_02032b28(task);
  }
  if (FRAME(task) == 180) {
    FRAME(task) = 0;
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    ResetLayersAndBlend();
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    ResetLayersAndBlend();
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    Task_ResumeScript(task);
  }
}
