#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Task_ResumeScript(Task* task);
void ResultStats_Draw(int offset, Task* task);
void ResultStats_AddButtons(void);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

#define SLIDE_FRAMES 24
/* This task keeps a frame counter in Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Task update of script data_0215a690: over SLIDE_FRAMES frames, eases
 * layers 0 and 1 of both screens in from 256 pixels to the right and
 * redraws the page sprites at the same offset. Then adds the page's menu
 * buttons and resumes the script. */
void ResultStats_SlideIn(Task* task) {
  int offset;
  Unk_027c0280* savedGfx;
  Unk_02080440* saved;

  FRAME(task)++;
  offset = Math_Ease(4, 256, 0, FRAME(task), SLIDE_FRAMES);
  savedGfx = data_027c0280;
  saved = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = -offset;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = -offset;
  Screen_SetCurrentBoth(data_027c0288.unk_04);
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = -offset;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = -offset;
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
  ResultStats_Draw(offset, task);
  if (FRAME(task) == SLIDE_FRAMES) {
    FRAME(task) = 0;
    ResultStats_AddButtons();
    Task_ResumeScript(task);
  }
}
