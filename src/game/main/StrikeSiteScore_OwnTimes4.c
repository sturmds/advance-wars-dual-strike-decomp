#include "types.h"

int StrikeSiteScore_OwnTimes4(int a0, int a1) {
  return (a1 == 0) ? -0x7fffffff : (a1 - (a0 << 2));
}
