#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Sprite_QueueCoPortrait(int x, int y, int arg2, int arg3, int list,
                            BOOL flip);
s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define BLEND_FRAMES 8

/* Step of the Super CO Power cut-in script, before SuperBreak_FlashUpdate.
 * Draws the power name letters and over BLEND_FRAMES frames moves the current
 * blend from EVA 8 and EVB 8 to fully the top layer. Then sets layer 0 to
 * blend over layers 1-3 and the sprites at EVA 0, shows BG0-BG3 and the
 * sprites, and resumes the script. */
void SuperBreak_BlendUpdate(Task* task) {
  int i;

  FRAME(task)++;
  Sprite_QueueCoPortrait(184, 160, 0x800, 16, 2, 0);
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 80, 0, 1);
  }
  data_027c0280->blendEva = Math_Ease(0, 8, 16, FRAME(task), BLEND_FRAMES);
  data_027c0280->blendEvb = Math_Ease(0, 8, 0, FRAME(task), BLEND_FRAMES);

  if (FRAME(task) == BLEND_FRAMES) {
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 16;
    /* Needed to match: the original stores blendEvy back unchanged. */
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
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
