#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"

extern MovePath* gMovePath;

/* Turns the drawn move path into one step per tile (PATH_STEP_*) in
 * pathSteps, ending with PATH_STEP_END. */
void MovePath_ToSteps(void) {
  s8 i;

  for (i = 1; i <= gMovePath->lastTile; i++) {
    if (gMovePath->x[i] < gMovePath->x[i - 1]) {
      data_027c027c->unk_48[data_027c0284]->pathSteps[i - 1] = PATH_STEP_LEFT;
    } else if (gMovePath->x[i] > gMovePath->x[i - 1]) {
      data_027c027c->unk_48[data_027c0284]->pathSteps[i - 1] = PATH_STEP_RIGHT;
    } else if (gMovePath->y[i] < gMovePath->y[i - 1]) {
      data_027c027c->unk_48[data_027c0284]->pathSteps[i - 1] = PATH_STEP_UP;
    } else {
      data_027c027c->unk_48[data_027c0284]->pathSteps[i - 1] = PATH_STEP_DOWN;
    }
  }
  data_027c027c->unk_48[data_027c0284]->pathSteps[i - 1] = PATH_STEP_END;
}
