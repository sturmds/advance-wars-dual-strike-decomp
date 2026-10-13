#include "types.h"

/* The packet taken from the queue (Link_PopUnflaggedPacket copies 24 bytes);
 * data_0229ca51 is its second byte. */
extern u8 data_0229ca50[24];
extern u8 data_0229ca51;

int Link_PopUnflaggedPacket(int player, void* dest);

/* Returns the next packet of the peer without its header byte, or NULL when none is queued. */
u8* Link_ReadUnflaggedPacket(int player) {
  if (Link_PopUnflaggedPacket(player, data_0229ca50) == 0) {
    return NULL;
  }
  return &data_0229ca51;
}
