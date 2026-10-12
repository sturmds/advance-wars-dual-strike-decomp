#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Chains the 16 list heads head to tail, cuts the chain after entries 4 and
 * 15, and makes the free pointer start at the pool again. */
void Sprites_ResetLists(void) {
  int i;

  for (i = 15; i >= 0; i--) {
    data_027c0280->spriteLists[i].next = &data_027c0280->spriteLists[i + 1];
    data_027c0280->spriteLists[i].unk_0c = NULL;
  }
  data_027c0280->spriteLists[15].next = NULL;
  data_027c0280->spriteLists[4].next = NULL;
  data_027c0280->spriteFree = data_027c0280->spritePool;
}
