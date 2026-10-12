#include "types.h"

extern u8 gLinkSlotPlayers[4];

/* The table holds player index + 1 for each link slot. Returns slot + 1, or 0 if the player has no slot. */
int Link_FindSlotOfPlayer(int player) {
  int slot;

  for (slot = 0; slot < 4; slot++) {
    if (gLinkSlotPlayers[slot] == player + 1) {
      return slot + 1;
    }
  }
  return 0;
}
