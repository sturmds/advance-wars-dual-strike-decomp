#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Chart damage with the player's firepower: (player, slot, powerState, unitType, targetType, weapon, targetDived). */
int Co_GetSlotChartDamage(int player, int slot, int powerState, int unitType,
                          int targetType, int weapon, BOOL targetDived);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Best damage of a unit type's two weapons against a target type that has
 * not dived, using the current player's front CO. */
int Ai_BestDamage(int unitType, int targetType, int powerState) {
  int primary = Co_GetSlotChartDamage(STATE->currentPlayer, 0, powerState,
                                      unitType, targetType, 0, FALSE);
  int secondary = Co_GetSlotChartDamage(STATE->currentPlayer, 0, powerState,
                                        unitType, targetType, 1, FALSE);

  if (primary <= secondary) {
    primary = secondary;
  }
  return primary;
}
