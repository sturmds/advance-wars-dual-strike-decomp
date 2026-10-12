#include "types.h"

/* Width of a score of `points` (WinLoseTask.scoreWidth): 24 plus 4 per digit after the first. */
int WinLose_GetScoreWidth(int points) {
  int width = 24;

  if (points >= 10000) {
    width += 16;
  } else if (points >= 1000) {
    width += 12;
  } else if (points >= 100) {
    width += 8;
  } else if (points >= 10) {
    width += 4;
  }
  return width;
}
