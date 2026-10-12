#include "types.h"

/* Like Battle_BobStep for the 192-step cycle used when the side's unk_48 is set: 1 at
 * phases 79, 80, 82, 85, 89 and 95, -1 at 175, 176, 178, 181, 185 and 191, 0 otherwise. */
int Battle_BobStepLong(int phase) {
  switch (phase) {
    case 79:
    case 80:
    case 82:
    case 85:
    case 89:
    case 95:
      return 1;
    case 175:
    case 176:
    case 178:
    case 181:
    case 185:
    case 191:
      return -1;
    default:
      return 0;
  }
}
