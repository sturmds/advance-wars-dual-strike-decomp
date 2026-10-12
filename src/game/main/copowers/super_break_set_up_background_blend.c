#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Step of the Super CO Power cut-in script: sets layer 2 to blend over layer 3 at
 * EVA 16, EVB 0, with no other targets, and restarts the frame counter. */
void SuperBreak_SetUpBackgroundBlend(Task* task) {
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = (u16)data_027c0280->blendEvy;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 0;
  FRAME(task) = 0;
}
