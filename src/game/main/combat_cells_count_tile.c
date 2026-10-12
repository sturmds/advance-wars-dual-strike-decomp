#include "types.h"
#include "game/Unk_02160ac0.h"

/* Counts the Combat mode cells with state 1 and this tile value, at most 9.
 * The score list uses it for the per-tile bonus rows. */
int CombatCells_CountTile(int tile) {
  int count = 0;
  int i;

  for (i = 0; i < data_02160ac0->cellCount; i++) {
    if (data_02160ac0->cells[i].state == 1 &&
        data_02160ac0->cells[i].tile == tile) {
      count++;
    }
  }
  if (count >= 10) {
    count = 9;
  }
  return count;
}
