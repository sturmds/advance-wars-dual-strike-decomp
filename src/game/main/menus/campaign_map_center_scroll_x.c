#include "types.h"

/* Horizontal scroll that puts x near the middle of the screen, kept
 * within 0-224. func_0204ec34 does the same for y (88, 0-48). */
int CampaignMap_CenterScrollX(int x) {
  x -= 120;
  if (x < 0) {
    return 0;
  }
  if (x > 224) {
    x = 224;
  }
  return x;
}
