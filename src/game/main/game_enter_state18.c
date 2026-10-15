#include "types.h"
#include "game/Unk_02080440.h"

/* Switches unk_18 to 18, remembering the old value in unk_1a. */
void Game_EnterState18(void) {
  if (MAP_STATE->unk_18 != 18) {
    MAP_STATE->unk_1a = MAP_STATE->unk_18;
    MAP_STATE->unk_18 = 18;
  }
}
