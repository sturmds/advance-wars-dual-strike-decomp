#include "types.h"
#include "game/Unk_02080440.h"

#define CURRENT_PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])

/* Entry test of the weather entry (handler Menu_PickNextWeather): 1 hides it,
 * and it is hidden while the front CO half of a tag power runs. */
s8 PowerMenu_PickNextWeatherHidden(void) {
  return CURRENT_PLAYER.tagPowerHalf == TAG_POWER_FRONT;
}
