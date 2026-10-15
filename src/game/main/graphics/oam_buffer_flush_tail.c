#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"

void Mem_Copy32(const void* src, void* dst, u32 size);
void Oam_HideEntries(
    void* entries,
    int count); /* moves OAM entries off screen (y = 192), 16 at a time */

/* Copies the sprites queued in the tail of the OAM buffer to the OAM copy,
 * then clears the tail for the next frame. */
void OamBuffer_FlushTail(void) {
  Mem_Copy32(data_027c0280->oamBuffer->tail.src,
             data_027c0280->oamBuffer->tail.dst,
             data_027c0280->oamBuffer->tail.count << 3);
  Oam_HideEntries(data_027c0280->oamBuffer->tail.src,
                  data_027c0280->oamBuffer->tail.count);
  data_027c0280->oamBuffer->unk_600 = data_027c0280->oamBuffer->tail.src;
  data_027c0280->oamBuffer->unk_608 = data_027c0280->oamBuffer;
  data_027c0280->oamBuffer->unk_608 = 0;
}
