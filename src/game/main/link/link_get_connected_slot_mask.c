#include "types.h"
#include "game/link_slots.h"

u16 Wireless_GetChildAids(void);

/* Bit n is set when the player in link slot n is connected (player 0 always counts). */
u8 Link_GetConnectedSlotMask(void) {
  int slot;
  u8 mask = 0;

  for (slot = 0; slot < 4; slot++) {
    if ((Wireless_GetChildAids() | 1) & (1 << (gLinkSlotPlayers[slot] - 1))) {
      mask |= 1 << slot;
    }
  }
  return mask;
}
