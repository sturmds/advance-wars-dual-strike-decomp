#include "types.h"

void Skills_Clear(u32* skills);

/* Copies a skill set (4 words) from `src` to `dest`; a NULL `src` leaves
 * `dest` cleared. */
void Skills_Copy(const u32* src, u32* dest) {
  int i;

  Skills_Clear(dest);
  if (src == NULL) {
    return;
  }
  for (i = 0; i < 4; i++) {
    dest[i] = src[i];
  }
}
