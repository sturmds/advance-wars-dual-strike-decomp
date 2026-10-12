#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Takes no argument; declared with one so that mode stays in r0 as the original does (needed to match). */
int AiTurn_IsPhase3(int mode);

/* With mode 1 and neither fog of war nor rain on the map, returns FALSE;
 * otherwise returns whether AiTurn_IsPhase3 says gAiTurnState has state 3.
 * No callers. */
BOOL func_02098e7c(int mode) {
  Unk_02080440_1c4* rules;

  if (mode == 1) {
    rules = STATE->unk_1c4;
    if (rules->fog == 0 && rules->weather != WEATHER_RAIN) {
      return FALSE;
    }
  }
  return AiTurn_IsPhase3(mode) == 1;
}
