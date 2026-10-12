#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"

extern Unk_027c0280* data_027c0280;

void Mem_Copy32(const void* src, void* dst, u32 size);
void Oam_HideEntries(void* oam, int count);

/* Copies the sprites queued in the head of the OAM buffer to the OAM copy,
 * then clears the head for the next frame. Does nothing when the head has
 * no entries. */
void OamBuffer_FlushHead(void) {
  if (data_027c0280->oamBuffer->head.count == 0) {
    return;
  }
  Mem_Copy32(data_027c0280->oamBuffer->head.src,
             data_027c0280->oamBuffer->head.dst,
             data_027c0280->oamBuffer->head.count << 3);
  Oam_HideEntries(data_027c0280->oamBuffer->head.src,
                  data_027c0280->oamBuffer->head.count);
  data_027c0280->oamBuffer->unk_604 = data_027c0280->oamBuffer->head.src;
}
