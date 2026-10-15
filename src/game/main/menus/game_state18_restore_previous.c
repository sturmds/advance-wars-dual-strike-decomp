#include "types.h"
#include "game/Unk_02080440.h"

void Game_State18_CheckEnd(void);

/* Handler for game state 18 (see Game_EnterState18): goes back to the state
 * it replaced, which unk_1a holds. */
void Game_State18_RestorePrevious(void) {
  MAP_STATE->unk_18 = MAP_STATE->unk_1a;
  MAP_STATE->unk_1a = 0;
  Game_State18_CheckEnd();
}
