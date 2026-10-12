#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"

extern Unk_027c0280* data_027c0280;

/* Queues a sprite made of several OBJs. sprite is a count followed by that
 * many (attribute 0, 1, 2) triples; x and y are added to each OBJ's
 * position and tileAdd to its tile number. */
void Oam_PutObjList(int x, int y, const u16* sprite, int tileAdd) {
  s16 count = *sprite++;
  u16* entry = data_027c0280->oamBuffer->unk_604;

  if (count == 0) {
    return;
  }
  do {
    u16 attr0 = sprite[0];
    u16 attr1;

    entry[0] = ((attr0 | y) & 0xff00) | ((attr0 + y) & 0xff);
    attr1 = sprite[1];
    entry[1] = ((attr1 | x) & 0xfe00) | ((attr1 + x) & 0x1ff);
    entry[2] = sprite[2] + tileAdd;
    sprite += 3;
    entry += 4;
    /* needed to match: the cursor is advanced in the buffer and in a local */
    *(u8**)&data_027c0280->oamBuffer->unk_604 += 8;
  } while (--count != 0);
}
