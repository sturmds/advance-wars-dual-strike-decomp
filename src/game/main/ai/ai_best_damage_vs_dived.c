#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Chart damage with the player's firepower: (player, slot, powerState, unitType, targetType, weapon, targetDived). */
int func_020e5e84(int player, int slot, int powerState, int unitType, int targetType, int weapon, BOOL targetDived);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Best damage of a unit type's two weapons against a dived target type (a
 * Sub or Stealth), using the current player's firepower. */
int Ai_BestDamageVsDived(int unitType, int targetType, int powerState)
{
    int primary = func_020e5e84(STATE->currentPlayer, 0, powerState, unitType, targetType, 0, TRUE);
    int secondary = func_020e5e84(STATE->currentPlayer, 0, powerState, unitType, targetType, 1, TRUE);

    if (primary <= secondary) {
        primary = secondary;
    }
    return primary;
}
