#include "types.h"
#include "game/Unk_02080440.h"

void Gfx_LoadPlayerBmap0a0Palette(int player);

void func_020f931c(void) {
  Unk_02080440* t0 = data_027c027c;
  int t1 = data_027c0284;
  Gfx_LoadPlayerBmap0a0Palette(t0->unk_48[t1]->currentPlayer);
}
