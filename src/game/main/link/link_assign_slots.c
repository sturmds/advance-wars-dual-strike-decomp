#include "types.h"

extern u8 gLinkSlotPlayers[4];

u16 Wireless_GetChildAids(void);

/* Fills gLinkSlotPlayers with player index + 1: the connected players first, then the others (player 0 always counts as connected). */
void Link_AssignSlots(void) {
  int count = 0;
  int i;
  u8* slot = gLinkSlotPlayers;

  for (i = 0; i < 4; i++) {
    if ((Wireless_GetChildAids() | 1) & (1 << i)) {
      *slot++ = i + 1;
      count++;
    }
  }
  slot = &gLinkSlotPlayers[count];
  for (i = 0; i < 4; i++) {
    if (((Wireless_GetChildAids() | 1) & (1 << i)) == 0) {
      *slot++ = i + 1;
      count++;
    }
  }
}
