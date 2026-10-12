#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"

extern Unk_027c0280* data_027c0280;

/* Splits the 128 OAM entries into a head of headCount entries and a tail of
 * the rest. */
void OamBuffer_Split(int headCount) {
  data_027c0280->oamBuffer->head.src = data_027c0280->oamBuffer;
  data_027c0280->oamBuffer->head.dst = data_027c0280->oamDest;
  data_027c0280->oamBuffer->head.byteOffset = 0;
  data_027c0280->oamBuffer->head.count = headCount;
  data_027c0280->oamBuffer->tail.src =
      &data_027c0280->oamBuffer->entries[headCount * 8];
  data_027c0280->oamBuffer->tail.dst = &data_027c0280->oamDest[headCount * 8];
  data_027c0280->oamBuffer->tail.byteOffset = headCount * 8;
  data_027c0280->oamBuffer->tail.count = OAM_ENTRY_COUNT - headCount;
}
