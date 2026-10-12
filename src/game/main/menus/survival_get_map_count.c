#include "types.h"

u16* Survival_GetMapList(int mode);

/* Number of maps in the run for the mode variant: the entries of its map
 * list before the 0 that ends it. */
int Survival_GetMapCount(int mode) {
  u16* maps = Survival_GetMapList(mode);
  int count = 0;

  while (*maps != 0) {
    count++;
    maps++;
  }
  return count;
}
