#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Queues a sprite at the head of sprite list list, using the next free
 * entry. x and y carry OAM attribute bits as well as the position. */
void Sprite_Queue(int list, int x, int y, const void* data, int unk08,
                  int unk0a) {
  data_027c0280->spriteFree->next = data_027c0280->spriteLists[list].next;
  data_027c0280->spriteFree->x = x;
  data_027c0280->spriteFree->y = y;
  data_027c0280->spriteFree->unk_08 = unk08;
  data_027c0280->spriteFree->unk_0c = data;
  data_027c0280->spriteFree->unk_0a = unk0a;
  data_027c0280->spriteLists[list].next = data_027c0280->spriteFree;
  data_027c0280->spriteFree++;
}
