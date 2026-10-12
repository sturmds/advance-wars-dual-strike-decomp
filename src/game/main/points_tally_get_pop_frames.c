#include "types.h"

/* How many frames the total's pop-in lasts: 24, plus 4 for each digit
 * after the first. */
int PointsTally_GetPopFrames(int total) {
  int frames = 24;

  if (total >= 1000) {
    frames += 12;
  } else if (total >= 100) {
    frames += 8;
  } else if (total >= 10) {
    frames += 4;
  }
  return frames;
}
