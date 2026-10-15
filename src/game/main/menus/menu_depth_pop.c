#include "types.h"
#include "game/Unk_02080440.h"

void MenuDepth_Pop(void) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];

  if (state->menuDepth != 0) {
    state->menuDepth--;
  }
}
