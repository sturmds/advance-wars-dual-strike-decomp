#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL CanUseSuperPower(int player, int slot);

#define STATE (data_027c027c->unk_48[data_027c0284])

BOOL func_020bfbe0(void) {
  if (STATE->unk_5c != 0 && CanUseSuperPower(STATE->currentPlayer, 0)) {
    return FALSE;
  }
  return TRUE;
}
