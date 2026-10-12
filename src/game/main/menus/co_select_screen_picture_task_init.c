#include "types.h"
#include "game/co_select_screen.h"

/* Starts both pictures at the cursor column's COs, fully on screen and
 * already loaded, with the slide-in flag set. */
void CoSelectPictureTask_Init(CoSelectPictureTask* task) {
  int row;

  for (row = 0; row <= 1; row++) {
    task->slideFrom[row] = 0;
    task->slideOffset[row] = 0;
    task->co[row] =
        (&data_0229c73c->coGrid[(data_0229c73c->cursorColumn - 1) * 2])[row];
    task->shownCo[row] = task->co[row];
    task->frame[row] = 8;
    task->waitFrames[row] = 0;
    task->slidingIn[row] = 1;
  }
}
