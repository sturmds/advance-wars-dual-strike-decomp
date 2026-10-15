#include "types.h"
#include "game/Unk_02080440.h"

BOOL CanUseSuperPower(int player, int slot);

BOOL func_020bfbe0(void) {
  if (MAP_STATE->unk_5c != 0 && CanUseSuperPower(MAP_STATE->currentPlayer, 0)) {
    return FALSE;
  }
  return TRUE;
}
