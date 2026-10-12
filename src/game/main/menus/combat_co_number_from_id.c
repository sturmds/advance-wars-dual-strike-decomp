#include "types.h"
#include "game/co_data.h"

/* Number of a Combat mode CO: 1 to 7 in the order of data_02160ae0 and
 * gCombatModeCos (Andy, Max, Sami, Flak, Lash, Adder, Hawke), 0 for any other
 * CO. Inverse of CombatCo_IdFromNumber. */
int CombatCo_NumberFromId(int coId) {
  switch (coId) {
    case CO_ANDY:
      return 1;
    case CO_MAX:
      return 2;
    case CO_SAMI:
      return 3;
    case CO_FLAK:
      return 4;
    case CO_LASH:
      return 5;
    case CO_ADDER:
      return 6;
    case CO_HAWKE:
      return 7;
    default:
      return 0;
  }
}
