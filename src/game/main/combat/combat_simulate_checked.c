#include "types.h"
#include "sdk/os.h"

void Combat_SimulateBattle(s16 attackerIndex, s16 defenderIndex, s16* pos);

/* Combat_SimulateBattle, but halts the game when defenderIndex is 0 (no
 * unit). */
void Combat_SimulateChecked(int attackerIndex, int defenderIndex, s16* pos) {
  if (defenderIndex == 0) {
    OS_Terminate();
    return;
  }
  Combat_SimulateBattle(attackerIndex, defenderIndex, pos);
}
