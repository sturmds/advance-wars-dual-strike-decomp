#include "types.h"
#include "game/Unk_02080440.h"

int Player_GetCoCostPercent(int player, int coSlot, int powerState,
                            int unitType);

int func_020df900(int player, int unitType) {
  return Player_GetCoCostPercent(
      player, 0,
      data_027c027c->unk_48[data_027c0284]->players[player].front.powerState,
      unitType);
}
