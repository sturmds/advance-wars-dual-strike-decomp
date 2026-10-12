#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

BOOL Player_IsInGame(u8 player);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Sets how many days a new weather lasts for every player still in the
 * game: two days for power snow, one for anything else. */
void Weather_SetDays(int weather) {
  int days;
  int player;

  switch (weather) {
    case WEATHER_SNOW:
    default:
      days = 1;
      break;
    case WEATHER_RAIN:
      days = 1;
      break;
    case WEATHER_SAND:
      days = 1;
      break;
    case WEATHER_POWER_SNOW:
      days = 2;
      break;
  }

  for (player = 1; player < 5; player++) {
    if (Player_IsInGame(player)) {
      data_027c027c->unk_5c[data_027c0284]->weatherDays[player] =
          (u8)days; /* needed to match: the cast */
    }
  }
}
