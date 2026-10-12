#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

/* Sets which way the slide task moves after the cursor moved. displayedRow
 * and displayedPage hold the cell shown before the move. A jump of more than
 * 20 cells counts as the other way round, as does a wrap. */
void Ov005SoundRoom_SetSlideDirection(Unk_ov005_0236bbac* state) {
  Ov005CoSlideTask* task = state->unk_50;
  int oldIndex;
  int newIndex;

  task->step = 1;
  oldIndex = state->displayedRow + state->displayedPage * 8;
  newIndex = state->selection + state->page * 8;
  if (state->cursorWrap == 0) {
    if (oldIndex < newIndex) {
      if (newIndex - oldIndex > 20) {
        task->slideBack = 1;
      } else {
        task->slideBack = 0;
      }
    } else {
      if (oldIndex - newIndex > 20) {
        task->slideBack = 0;
      } else {
        task->slideBack = 1;
      }
    }
  } else if (state->cursorWrap == 1) {
    task->slideBack = 1;
  } else {
    task->slideBack = 0;
  }
}
