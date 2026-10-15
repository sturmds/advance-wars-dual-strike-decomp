#include "types.h"
#include "game/Unk_02080440.h"

void Player_SetUnk33(int, void*);

void func_020dd370(void) {
  Unk_02080440* t0 = data_027c027c;
  int t1 = data_027c0284;
  Player_SetUnk33(t0->unk_48[t1]->currentPlayer, t0);
}
