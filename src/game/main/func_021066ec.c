#include "types.h"

extern int gCurrentSoundArc;

int func_021066ec(int a0) {
  int t0 = gCurrentSoundArc;
  gCurrentSoundArc = a0;
  return t0;
}
