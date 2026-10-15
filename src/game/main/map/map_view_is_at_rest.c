#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when no view scroll is in progress and the cursor sits on a whole
 * tile (cursorPixelX and cursorPixelY have their low 4 bits clear). */
BOOL MapView_IsAtRest(void) {
  Unk_02080440_48* state = MAP_STATE;

  if ((state->unk_1c4->scrollFlags & 15) == 0 &&
      ((s16)state->cursorPixelX & 15) == 0 &&
      ((s16)state->cursorPixelY & 15) == 0) {
    return TRUE;
  }
  return FALSE;
}
