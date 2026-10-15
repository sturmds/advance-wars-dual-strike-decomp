#include "types.h"
#include "game/Unk_02080440.h"
#include "game/screen.h"

BOOL func_020ba2b8(void);
BOOL func_020ba208(void);

/* Handler for game state 5. Switches to state 6, then starts one of the two
 * tasks above (func_020ba2b8 first, else func_020ba208). */
void Game_State5_StartTurnTask(void) {
  if (SceneMode_IsChanging()) {
    return;
  }
  MAP_STATE->unk_18 = 6;
  if (func_020ba2b8()) {
    return;
  }
  func_020ba208();
}
