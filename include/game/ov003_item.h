#ifndef GAME_OV003_ITEM_H
#define GAME_OV003_ITEM_H

#include "types.h"

#define OV003_ITEM_COUNT 4

/* One of the 4 8-byte entries in data_ov003_02369b98 (overlay 3): a pickup.
 * Ov003_SpawnItem drops one at a position (5 in 6 of the time, kind 1 to 5);
 * a unit within 48 of its position picks it up. A free entry has timer 0. */
typedef struct Ov003Item {
  s16 x; /* 8 per tile, like Ov003Object */
  s16 y;
  u16 timer; /* frames left; 600 when it appears */
  u8 kind;   /* 1 to 5: picks the sprite and the effect (Ov003_ApplyPickup) */
  u8 pad_07;
} Ov003Item;

typedef char Ov003Item_size_check[sizeof(Ov003Item) == 8 ? 1 : -1];

extern Ov003Item data_ov003_02369b98[OV003_ITEM_COUNT];

#endif
