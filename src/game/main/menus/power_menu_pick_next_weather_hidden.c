#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CURRENT_PLAYER (STATE->players[STATE->currentPlayer])

/* Entry test of the weather entry (handler Menu_PickNextWeather): 1 hides it,
 * and it is hidden while the front CO half of a tag power runs. */
s8 PowerMenu_PickNextWeatherHidden(void) {
  return CURRENT_PLAYER.tagPowerHalf == TAG_POWER_FRONT;
}
