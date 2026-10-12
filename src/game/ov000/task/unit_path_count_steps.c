#include "types.h"

/* Counts the move steps (commands 0 to 3) of a unit path, which ends at -1 or 4.
 * Command 9 is followed by a parameter byte. */
int UnitPath_CountSteps(const s8* path) {
  int i = 0;
  int steps = 0;

  for (;;) {
    switch (path[i++]) {
      case 9:
        i++;
        break;
      case -1:
      case 4:
        return steps;
      case 0:
      case 1:
      case 2:
      case 3:
        steps++;
        break;
      case 10:
        break;
    }
  }
}
