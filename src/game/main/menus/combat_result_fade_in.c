#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void* state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);

#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Task step of the Combat result screen: counts the frames of a 32 frame
 * fade-in (the colour effect level, blendEvy, eases from 16 to 0 on both
 * screens), then resumes the script. */
void CombatResult_FadeIn(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  FRAME(task)++;
  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->blendEvy = Math_Ease(0, 16, 0, FRAME(task), 32);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->blendEvy = Math_Ease(0, 16, 0, FRAME(task), 32);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
  if (FRAME(task) == 32) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
