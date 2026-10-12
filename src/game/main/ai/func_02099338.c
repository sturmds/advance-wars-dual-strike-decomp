#include "types.h"
#include "game/ai_unit_action.h"

BOOL func_02099528(void);

/* When func_02099528 says so, sets the AI phase to 3 and returns TRUE.
 * Nothing calls it. */
BOOL func_02099338(void) {
  if (func_02099528() == 1) {
    gAiTurnState->phase = 3;
    return TRUE;
  }
  return FALSE;
}
