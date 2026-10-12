#ifndef GAME_OV003_PICKUP_H
#define GAME_OV003_PICKUP_H

#include "types.h"

/* One of the 4 8-byte items in data_ov003_02369b98 (overlay 3). Ov003_SpawnItem
 * drops one at a position (5 in 6 of the time, kind 1 to 5); one with time
 * 0 is free. A unit within 48 of its position picks it up. */
typedef struct Ov003Pickup {
  s16 x;
  s16 y;
  u16 time; /* frames left; 600 when dropped */
  u8 kind;  /* 1 to 5, see Ov003_ApplyPickup */
  u8 pad_07;
} Ov003Pickup;

extern Ov003Pickup data_ov003_02369b98[4];

#endif
