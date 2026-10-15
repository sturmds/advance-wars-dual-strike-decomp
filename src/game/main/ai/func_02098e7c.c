#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/player.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/ai/ai_power_check.h"

/* Needed to match: the original passes mode on to AiTurn_IsPhase3, which
 * ignores it, so this file declares it with an argument. */
int AiTurn_IsPhase3(int mode);

/* With mode 1 and neither fog of war nor rain on the map, returns FALSE;
 * otherwise returns whether AiTurn_IsPhase3 says gAiTurnState has state 3.
 * No callers. */
BOOL func_02098e7c(int mode) {
  Unk_02080440_1c4* rules;

  if (mode == 1) {
    rules = MAP_STATE->unk_1c4;
    if (rules->fog == 0 && rules->weather != WEATHER_RAIN) {
      return FALSE;
    }
  }
  return AiTurn_IsPhase3(mode) == 1;
}
