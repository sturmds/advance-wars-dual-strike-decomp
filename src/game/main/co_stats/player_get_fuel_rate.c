#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

/* Fuel burned per movement point: 2 in snow, 1 otherwise. With CO effects
 * on, a player whose front CO is Olaf always gets 1. */
int Player_GetFuelRate(int playerIndex) {
  int rate = 1;

  if (data_027c027c->unk_48[data_027c0284]->unk_1c4->weather == WEATHER_SNOW) {
    rate = 2;
  }
  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return rate;
  }
  if ((data_027c027c->unk_48[data_027c0284]->players[playerIndex].front.id &
       PLAYER_CO_ID_MASK) == CO_OLAF) {
    rate = 1;
  }
  return rate;
}
