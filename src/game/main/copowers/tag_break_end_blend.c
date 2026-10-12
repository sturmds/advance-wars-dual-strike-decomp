#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
s16 BreakText_GetCentredX(u8 slot);
void BreakText_QueueLetterSized(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define SET_BLEND_WEIGHTS(eva, evb) \
  data_027c0280->blendEva = (eva);  \
  data_027c0280->blendEvb = (evb)

/* Two-frame step of the tag power cut-in: redraws the task->unk_58 text
 * sprites at y 48 and sets the blend weights of both screens to 15/2, then on
 * the next frame to 16/0 (top layer only) and resumes the script. */
void TagBreak_EndBlend(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;

  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetterSized(i, BreakText_GetCentredX(i) & 0x1ff, 48, 0, 0);
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
