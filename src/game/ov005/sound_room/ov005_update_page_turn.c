#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bbac.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);
void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;
extern Unk_ov005_0236bbac* data_ov005_0236bbac;

/* Page turn on layer 0 of the second screen. Steps 1-2 ease the layer's
 * x scroll to -224 when task->entryIndex is set, to 224 otherwise;
 * step 3 copies selection and page into displayedRow and displayedPage, steps 4-5
 * scroll back to 0 from the other side. On frames with bit 1 set,
 * objScaleBoth eases between 256 and 320 too. */
void Ov005_UpdatePageTurn(Ov005Task* task) {
  Unk_ov005_0236bbac* state = data_ov005_0236bbac;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int x;

  switch (task->step) {
    case 0:
      state->objScaleBoth = -1;
      break;
    case 1:
      task->frame = 0;
      task->frames = 8;
      task->step++;
    case 2:
      if (task->entryIndex != 0) {
        x = Math_Ease(0, 0, -0xe0000, task->frame, task->frames);
      } else {
        x = Math_Ease(0, 0, 0xe0000, task->frame, task->frames);
      }
      if (task->frame & 2) {
        state->objScaleBoth = Math_Ease(0, 256, 320, task->frame, task->frames);
      }
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->bgScrollX[data_027c0280->layer0Bg] = x >> 12;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step++;
      }
      break;
    case 3:
      state->displayedRow = state->selection;
      state->displayedPage = state->page;
      task->step++;
      break;
    case 4:
      task->frame = 0;
      task->frames = 8;
      task->step++;
    case 5:
      if (task->entryIndex != 0) {
        x = Math_Ease(0, 0xe0000, 0, task->frame, task->frames);
      } else {
        x = Math_Ease(0, -0xe0000, 0, task->frame, task->frames);
      }
      if (task->frame & 2) {
        state->objScaleBoth = Math_Ease(0, 320, 256, task->frame, task->frames);
      }
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_04);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_04);
      data_027c0280->bgScrollX[data_027c0280->layer0Bg] = x >> 12;
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
      if (task->frame < task->frames) {
        task->frame++;
      } else {
        task->step = 0;
      }
      break;
  }
}
