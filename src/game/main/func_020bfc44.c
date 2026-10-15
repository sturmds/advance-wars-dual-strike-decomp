#include "types.h"
#include "game/Unk_02080440.h"

BOOL CanUseCoPowerStrict(int player, int slot);

BOOL func_020bfc44(void) {
  if (MAP_STATE->unk_5c != 0 &&
      CanUseCoPowerStrict(MAP_STATE->currentPlayer, 0)) {
    return FALSE;
  }
  return TRUE;
}
