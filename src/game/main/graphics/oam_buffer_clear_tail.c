#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"

void Oam_HideEntries(
    void* entries,
    int count); /* moves OAM entries off screen (y = 192), 16 at a time */

/* Throws away the sprites queued in the tail of the OAM buffer without
 * uploading them. The first store to unk_608 is overwritten at once; needed
 * to match: the original has it. */
void OamBuffer_ClearTail(void) {
  Oam_HideEntries(data_027c0280->oamBuffer->tail.src,
                  data_027c0280->oamBuffer->tail.count);
  data_027c0280->oamBuffer->unk_600 = data_027c0280->oamBuffer->tail.src;
  data_027c0280->oamBuffer->unk_608 = data_027c0280->oamBuffer;
  data_027c0280->oamBuffer->unk_608 = 0;
}
