#include "types.h"
#include "game/co_data.h"

/* CO id of a Combat mode CO number (1 to 7, see CombatCo_NumberFromId); 0 for any
 * other number. */
int CombatCo_IdFromNumber(int index) {
  switch (index) {
    case 1:
      return CO_ANDY;
    case 2:
      return CO_MAX;
    case 3:
      return CO_SAMI;
    case 4:
      return CO_FLAK;
    case 5:
      return CO_LASH;
    case 6:
      return CO_ADDER;
    case 7:
      return CO_HAWKE;
    default:
      return CO_NONE;
  }
}
