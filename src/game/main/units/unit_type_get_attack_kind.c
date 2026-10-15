#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

int GetUnitMaxRange(int player, int unitType);

/* How the current player's unitType attacks (ATTACK_KIND_*), from its
 * maximum range after CO bonuses. Units without a weapon are split into
 * Black Bomb, Oozium and the rest. */
int UnitType_GetAttackKind(u8 unitType) {
  int range = GetUnitMaxRange(
      data_027c027c->unk_48[data_027c0284]->currentPlayer, unitType);

  if (range > 1) {
    return ATTACK_KIND_INDIRECT;
  }
  if (range == 1) {
    return ATTACK_KIND_DIRECT;
  }
  if (unitType == UNIT_BLACK_BOMB) {
    return ATTACK_KIND_BLACK_BOMB;
  }
  if (unitType == UNIT_OOZIUM) {
    return ATTACK_KIND_OOZIUM;
  }
  return ATTACK_KIND_NONE;
}
