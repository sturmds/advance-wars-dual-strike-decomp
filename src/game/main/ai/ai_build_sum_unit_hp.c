#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u16
    gAiEnemyHpByType[UNIT_TYPE_COUNT]; /* per unit type: hp of enemy units */
extern u16 gAiOwnHpByType
    [UNIT_TYPE_COUNT]; /* per unit type: hp of the current player's team */

BOOL Player_IsInGame(u8 player);
void AiBuild_WeighUnitTypes(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Adds up unit hp per unit type, once for the current player's team and once
 * for the players on other teams, then calls AiBuild_WeighUnitTypes. Each player
 * owns slots first + 1 to first + 50 of Unk_02080440_48.units. */
void AiBuild_SumUnitHp(void) {
  /* needed to match: each loop has its own counter */
  int type;
  int player;
  int slot;
  int enemySlot;
  MapUnit* unit;

  for (type = 0; type < UNIT_TYPE_COUNT; type++) {
    gAiOwnHpByType[type] = gAiEnemyHpByType[type] = 0;
  }
  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (STATE->players[STATE->currentPlayer].team ==
        STATE->players[player].team) {
      for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
        unit = &STATE->units[slot];
        if (unit->type != UNIT_NONE) {
          gAiOwnHpByType[unit->type] += unit->hp;
        }
      }
    } else {
      for (enemySlot = Ov000_gPlayerFirstUnitSlot[player] + 1;
           enemySlot < Ov000_gPlayerFirstUnitSlot[player] + 51; enemySlot++) {
        unit = &STATE->units[enemySlot];
        if (unit->type != UNIT_NONE) {
          gAiEnemyHpByType[unit->type] += unit->hp;
        }
      }
    }
  }
  AiBuild_WeighUnitTypes();
}
