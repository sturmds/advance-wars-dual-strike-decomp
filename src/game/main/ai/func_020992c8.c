#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE with fog of war or rain on the map. Otherwise FALSE for mode 2 in AI
 * phase 3, TRUE for the rest. Nothing calls it. */
BOOL func_020992c8(int mode) {
  Unk_02080440_1c4* rules = STATE->unk_1c4;

  if (rules->fog != 0 || rules->weather == WEATHER_RAIN) {
    return TRUE;
  }
  if (mode == 2 && gAiTurnState->phase == 3) {
    return FALSE;
  }
  return TRUE;
}
