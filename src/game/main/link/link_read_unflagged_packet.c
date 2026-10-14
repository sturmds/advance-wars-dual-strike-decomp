#include "types.h"
#include "game/link_buffers.h"
#include "game/Unk_0215fe8c.h"

/* The packet taken from the queue (Link_PopUnflaggedPacket copies 24 bytes);
 * data_0229ca51 is its second byte. */
u8 data_0229ca50[24];
Unk_0215fe8c data_0229ca68;
Unk_0229ca88 data_0229ca88;
u8 data_0229cb4c[7][98];
u8 data_0229cdfc[7][386];
extern u8 data_0229ca51;

int Link_PopUnflaggedPacket(int player, void* dest);

/* Returns the next packet of the peer without its header byte, or NULL when none is queued. */
u8* Link_ReadUnflaggedPacket(int player) {
  if (Link_PopUnflaggedPacket(player, data_0229ca50) == 0) {
    return NULL;
  }
  return &data_0229ca51;
}
