#include "types.h"

/* Change of the bobbing angle of an HP cell on an air side (BattleCellTask.angle) at this
 * phase of its 144-step cycle: 1 at phases 70 and 71, -1 at 142 and 143, 0 otherwise. */
int Battle_BobStep(int phase) {
  switch (phase) {
    case 70:
    case 71:
      return 1;
    case 142:
    case 143:
      return -1;
    default:
      return 0;
  }
}
