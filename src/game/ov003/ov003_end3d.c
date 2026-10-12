#include "types.h"

/* Pops the matrix that Ov003_Begin3d pushed (MTX_POP). */
int Ov003_End3d(void) {
  *((int*)0x4000448) = 1;
  return 0x4000448;
}
