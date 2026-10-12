/* Built at optimization level 2 (the pragma sweep found it; level 4 strength-reduces the loops). */
#pragma optimization_level 2
#include "types.h"
#include "game/move_path.h"

/* Reverses the steps in [start, end) in place and ends the list with PATH_STEP_END
 * after them. Callers pass a list of at most 12 steps. */
void UnitPath_ReverseSteps(s8* start, s8* end) {
  s8 reversed[24];
  int count = 0;
  int i;

  if (end != start) {
    do {
      end--;
      reversed[count] = *end;
      count++;
    } while (end != start);
  }
  reversed[count] = PATH_STEP_END;
  i = 0;
  while (1) {
    *start = reversed[i];
    i++;
    if (*start == PATH_STEP_END) {
      break;
    }
    start++;
  }
}
