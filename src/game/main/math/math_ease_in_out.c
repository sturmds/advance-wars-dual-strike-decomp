#include "types.h"

int Math_Ease(int curve, int from, int to, int elapsed, int duration);

/* Moves from from to to over duration: the first half eases in (curve 1),
 * the second half eases out (curve 4). The midpoint is computed in each
 * branch to match. */
int Math_EaseInOut(int from, int to, int elapsed, int duration) {
  int half = duration >> 1;

  if (elapsed < half) {
    return Math_Ease(1, from, from + ((to - from) >> 1), elapsed, half);
  }
  return Math_Ease(4, from + ((to - from) >> 1), to, elapsed - half, half);
}
