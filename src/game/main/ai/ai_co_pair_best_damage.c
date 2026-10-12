#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Chart damage with the player's firepower: (player, slot, powerState, unitType, targetType, weapon, targetDived). */
int Co_GetSlotChartDamage(int player, int slot, int powerState, int unitType,
                          int targetType, int weapon, BOOL targetDived);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Best damage of a unit type's two weapons against a target type that has
 * not dived, for the current player. With a back CO, each weapon's damage
 * is the average of the front and back CO's. */
int Ai_CoPairBestDamage(int unitType, int targetType, int powerState) {
  int primary = Co_GetSlotChartDamage(STATE->currentPlayer, 0, powerState,
                                      unitType, targetType, 0, FALSE);
  int secondary = Co_GetSlotChartDamage(STATE->currentPlayer, 0, powerState,
                                        unitType, targetType, 1, FALSE);
  BOOL hasBackCo = STATE->players[STATE->currentPlayer].back.id ? TRUE : FALSE;

  if (hasBackCo == TRUE) {
    primary += Co_GetSlotChartDamage(STATE->currentPlayer, 1, powerState,
                                     unitType, targetType, 0, FALSE);
    secondary += Co_GetSlotChartDamage(STATE->currentPlayer, 1, powerState,
                                       unitType, targetType, 1, FALSE);
    primary >>= 1;
    secondary >>= 1;
  }
  if (primary > secondary) {
    return primary;
  }
  return secondary;
}
