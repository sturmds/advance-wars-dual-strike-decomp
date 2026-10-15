#include "types.h"
#include "game/Unk_02080440.h"

void func_020ccd7c(void);

/* Handler for game state 9: calls func_020ccd7c, then switches to state 12.
 * The name is a guess; what the call does is unknown. */
void Game_State9_RunStep(void) {
  func_020ccd7c();
  MAP_STATE->unk_18 = 12;
}
