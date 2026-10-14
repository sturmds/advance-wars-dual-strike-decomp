#include "types.h"
#include "game/battle_save.h"

extern u8 gGameFlagsLow[];

void func_0210187c(void) {
  int i;

  for (i = 0; i < GAME_FLAGS_LOW_SIZE; i++) {
    gGameFlagsLow[i] = 0;
  }
}
