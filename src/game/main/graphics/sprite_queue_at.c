#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Same as Sprite_Queue, but x and y are plain positions: they are cut to the
 * OAM position fields (x 9 bits, y 8 bits) so no attribute bits get in. */
void Sprite_QueueAt(int list, int x, int y, const void* data, int unk08,
                    int unk0a) {
  data_027c0280->spriteFree->next = data_027c0280->spriteLists[list].next;
  data_027c0280->spriteFree->x = x & 0x1ff;
  data_027c0280->spriteFree->y = y & 0xff;
  data_027c0280->spriteFree->unk_08 = unk08;
  data_027c0280->spriteFree->unk_0c = data;
  data_027c0280->spriteFree->unk_0a = unk0a;
  data_027c0280->spriteLists[list].next = data_027c0280->spriteFree;
  data_027c0280->spriteFree++;
}
