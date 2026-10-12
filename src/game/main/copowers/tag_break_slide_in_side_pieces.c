#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

#define SLIDE_FRAMES 128
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Screen_SetCurrentBoth(void* state);
void Task_ResumeScript(Task* task);
void TagBreak_DrawSidePieceFacing(int x, int y, int index);
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Script task step, 128 frames: scrolls BG3 of both screens down by 32 lines
 * and slides two TagBreak_DrawSidePieceFacing sprites in from below. On the last frame both
 * screens hide BG0, show BG1-BG3 and OBJ, and alpha-blend BG1 and BG2 over
 * BG3 (eva 0, evb 16: BG1 and BG2 not visible yet). Then the script resumes. */
void TagBreak_SlideInSidePieces(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int offset;

  FRAME(task)++;
  offset = Math_Ease(0, -32, 0, FRAME(task), SLIDE_FRAMES);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = offset + 192;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = offset;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  offset = Math_Ease(4, -96, 0, FRAME(task), SLIDE_FRAMES);
  TagBreak_DrawSidePieceFacing(64, offset + 224, 0);
  TagBreak_DrawSidePieceFacing(192, 224 - offset, 1);

  if (FRAME(task) == SLIDE_FRAMES) {
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c0280->dispCnt &= ~0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 16;
    /* Needed to match: the original reads and writes blendEvy back; without
     * the cast mwcc drops the self-assignment. */
    data_027c0280->blendEvy = (u16)data_027c0280->blendEvy;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendBottomObj = 0;
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c0280->dispCnt &= ~0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 16;
    data_027c0280->blendEvy = (u16)data_027c0280->blendEvy;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendBottomObj = 0;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    FRAME(task) = 0;
    Task_ResumeScript(task);
    /* Needed to match: with an early return for the other frames, or with
     * no return here, mwcc moves the saved027c argument copy after the
     * blend constants. */
    return;
  }
}
