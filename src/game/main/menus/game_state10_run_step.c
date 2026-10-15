#include "types.h"
#include "game/Unk_02080440.h"

void func_020cce44(void);

/* Handler for game state 10: calls func_020cce44, then switches to state 11.
 * The name is a guess; what the call does is unknown. */
void Game_State10_RunStep(void) {
  func_020cce44();
  MAP_STATE->unk_18 = 11;
}
