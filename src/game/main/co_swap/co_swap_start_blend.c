#include "types.h"
#include "game/co_swap.h"
#include "game/screen_other_map.h"
#include "game/win_quote.h"

s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* Script step of the CO swap cut-in (UnitAction_SwapCo): keeps the CO
 * pictures and the title in place for 8 frames, then alpha-blends BG0 over
 * BG1-BG3 and OBJ with BG0 not visible yet (eva 0, evb 16), turns all
 * layers on and resumes the script. */
void CoSwap_StartBlend(Task* task) {
  int i;

  CO_SWAP_FRAME(task)++;
  Sprite_QueueCoPortrait(CO_SWAP_FRONT_X, CO_SWAP_CO_Y, 0, 16, 2, FALSE);
  Sprite_QueueCoPortrait(CO_SWAP_BACK_X, CO_SWAP_CO_Y, 0x1000, 0x1c0, 3, FALSE);
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, CO_SWAP_TITLE_Y,
                          0, 1);
  }
  if (CO_SWAP_FRAME(task) != 8) {
    return;
  }
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = (u16)data_027c0280->blendEvy;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  data_027c0280->dispCnt |= 0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  CO_SWAP_FRAME(task) = 0;
  Task_ResumeScript(task);
}
