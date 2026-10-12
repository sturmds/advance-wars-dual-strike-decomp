#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
void TagBreak_DrawSidePiece(int x, int y, int side);
s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define SET_BLEND_WEIGHTS(eva, evb) \
  data_027c0280->blendEva = (eva);  \
  data_027c0280->blendEvb = (evb)

/* Two-frame step of the tag power cut-in, the same as TagBreak_EndBlend
 * but with the two side pieces (TagBreak_DrawSidePiece) at rest and the
 * task->unk_58 text sprites at y 80: blend weights of both screens go to
 * 15/2, then on the next frame to 16/0 and the script resumes. */
void TagBreak_EndBlendWithSidePieces(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;

  TagBreak_DrawSidePiece(72, 224, 0);
  TagBreak_DrawSidePiece(184, 224, 1);
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, 80, 0, 0);
  }
  if (FRAME(task) == 0) {
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    SET_BLEND_WEIGHTS(15, 2);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    SET_BLEND_WEIGHTS(15, 2);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  } else {
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    SET_BLEND_WEIGHTS(16, 0);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    SET_BLEND_WEIGHTS(16, 0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    FRAME(task) = 0;
    Task_ResumeScript(task);
    return;
  }
  FRAME(task)++;
}
