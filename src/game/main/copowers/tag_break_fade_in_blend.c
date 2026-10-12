#include "types.h"
#include "game/tag_break_task.h"

void Screen_SetCurrentBoth(void* state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

/* Frame 0 sets eva to 1; frames 1-32 ease eva from 2 to 14 and evb from 16
 * to 4 over 31 frames. */
#define SET_FADE(task)                                                    \
  if ((task)->frame == 0) {                                               \
    data_027c0280->blendEva = 1;                                          \
  } else {                                                                \
    data_027c0280->blendEva = Math_Ease(4, 2, 14, (task)->frame - 1, 31); \
    data_027c0280->blendEvb = Math_Ease(4, 16, 4, (task)->frame - 1, 31); \
  }

/* Step of the tag power cut-in: raises the blend weights of both screens
 * while the frame scrolls, then resumes the script after frame 32. */
void TagBreak_FadeInBlend(TagBreakTask* task) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c = data_027c027c;

  Screen_SetCurrentBoth(data_027c0288.unk_00);
  SET_FADE(task);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  SET_FADE(task);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  TagBreakTask_ScrollFrame(task);
  if (task->frame != 32) {
    task->frame++;
  } else {
    task->frame = 0;
    Task_ResumeScript((Task*)task);
  }
}
