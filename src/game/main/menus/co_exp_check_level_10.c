#include "types.h"
#include "game/co_exp.h"
#include "game/event_script.h"
#include "game/menu_state.h"
#include "game/unlocks.h"
#include "game/win_lose.h"

extern EventScriptCommand data_02159e60[];

/* On the results screen, starts an event script once any CO of side A
 * (two per player, up to six) is at level 10 or more. Game flag 40 keeps
 * the script from playing again. */
void CoExp_CheckLevel10(void) {
  int i;
  int count = 0;
  int limit;

  if (GameFlag_IsSet(40)) {
    return;
  }
  limit = data_022a7a88.countA * 2;
  for (i = 0; i < limit; i++) {
    if (i >= 6) {
      break;
    }
    if (gMenuState.coExp.exp[i].level >= 10) {
      count++;
    }
  }
  if (count == 0) {
    return;
  }
  GameFlag_Set(40, 1);
  EventScript_Start(data_02159e60);
}
