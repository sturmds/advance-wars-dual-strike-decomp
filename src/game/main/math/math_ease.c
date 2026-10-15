#include "types.h"
#include "game/math.h"

/* Value between from and to after elapsed of duration steps. Curves:
 * 0 linear; 1, 2, 3 start slowly (elapsed squared, cubed, to the fourth);
 * 4, 5 end slowly (the same with the time left, squared and cubed).
 * Duration 0 gives to; an unknown curve gives 0. */
int Math_Ease(int curve, int from, int to, int elapsed, int duration) {
  int left;

  if (duration == 0) {
    return to;
  }
  switch (curve) {
    case 0:
      return from + HwDiv32(elapsed * (to - from), duration);
    case 1:
      return from +
             HwDiv32((to - from) * (elapsed * elapsed), duration * duration);
    case 2:
      return from + HwDiv32((to - from) * (elapsed * (elapsed * elapsed)),
                            duration * (duration * duration));
    case 3:
      return from +
             HwDiv32((to - from) * (elapsed * (elapsed * (elapsed * elapsed))),
                     duration * (duration * (duration * duration)));
    case 4:
      left = duration - elapsed;
      return to - HwDiv32((to - from) * (left * left), duration * duration);
    case 5:
      left = duration - elapsed;
      return to - HwDiv32((to - from) * (left * (left * left)),
                          duration * (duration * duration));
    default:
      return 0;
  }
}
