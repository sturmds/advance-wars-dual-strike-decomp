#include "types.h"
#include "game/task.h"
#include "game/screen_other_map.h"
#include "game/math.h"

void Screen_SetCurrentBoth(void* state);
s16 BreakText_GetScreenX(u8 slot);
void BreakText_QueueLetter(u8 slot, u16 x, int y, int attr, int arg);
void Task_ResumeScript(Task* task);

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define FADE_FRAMES 32

/* Last step of the Tag Power cut-in before it ends: keeps drawing the
 * task->unk_58 letter sprites at y 80 and raises the master brightness of
 * both screens to white (16) over 32 frames, then resumes the script. */
void TagPowerScreen_FadeToWhite(Task* task) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  int i;

  FRAME(task)++;
  for (i = 0; i < task->unk_58; i++) {
    BreakText_QueueLetter(i, BreakText_GetScreenX(i) & 0x1ff, 80, 0, 0);
  }
  savedGfx = data_027c0280;
  savedState = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->masterBrightness =
      Math_Ease(4, 0, 16, FRAME(task), FADE_FRAMES);
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->masterBrightness =
      Math_Ease(4, 0, 16, FRAME(task), FADE_FRAMES);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
  if (FRAME(task) == FADE_FRAMES) {
    FRAME(task) = 0;
    Task_ResumeScript(task);
  }
}
