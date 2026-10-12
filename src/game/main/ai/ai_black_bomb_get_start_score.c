#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Unit_GetValuePerHp(int playerIndex, int unitType);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Score a Black Bomb gives for exploding: half the value of its hp. Map
 * 226 always gives 1. */
u32 AiBlackBomb_GetStartScore(void) {
  int value;

  if (STATE->unk_1c4->mapId == 226) {
    return 1;
  }
  value = Unit_GetValuePerHp(STATE->currentPlayer, UNIT_BLACK_BOMB);
  return (u32)(value * STATE->actingUnit->hp) / 2;
}
