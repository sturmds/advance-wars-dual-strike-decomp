#include "types.h"

#define CO_PICK_LIST_END 0xff

extern u8 gCoPickList[];

u32 Rng_Next(void);

/* A random CO id from the pick list that BuildCoPickList filled. */
u8 CoPickList_PickRandom(void) {
  u8 count = 0;

  while (gCoPickList[count] != CO_PICK_LIST_END) {
    count++;
  }
  return gCoPickList[Rng_Next() % count];
}
