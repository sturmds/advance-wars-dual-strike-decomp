#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the AI phase is 32 with fog of war or rain on the map, or 27
 * without. Called by func_02099338. */
BOOL func_02099528(void) {
  Unk_02080440_1c4* rules = STATE->unk_1c4;

  if (rules->fog != 0 || rules->weather == WEATHER_RAIN) {
    if (gAiTurnState->phase == 32) {
      return TRUE;
    }
  } else if (gAiTurnState->phase == 27) {
    return TRUE;
  }
  return FALSE;
}
