#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

int Rng_RollTier(u32 limit1, u32 limit2, u32 limit3);
BOOL Weather_TickDays(void);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)
#define WEATHER (data_027c027c->unk_5c[data_027c0284])

/* Weather for the next turn. Clear weather stays clear unless the map's
 * weather setting is random (randomWeather is 1); then the weather record gives the percent
 * chances of snow, rain and sand. Other
 * weather lasts until Weather_TickDays reports that every player's days ran
 * out, then returns to the map's default (defaultWeather). */
int Weather_PickNext(void) {
  int weather = VIEW->weather;

  if (weather == WEATHER_CLEAR) {
    if (VIEW->randomWeather != 1) {
      return weather;
    }
    switch (Rng_RollTier(
        WEATHER->snowChance, WEATHER->snowChance + WEATHER->rainChance,
        WEATHER->sandChance + (WEATHER->snowChance + WEATHER->rainChance))) {
      case 1:
        weather = WEATHER_SNOW;
        break;
      case 2:
        weather = WEATHER_RAIN;
        break;
      case 3:
        weather = WEATHER_SAND;
        break;
    }
  } else {
    if (weather == VIEW->defaultWeather && VIEW->randomWeather == 0) {
      return weather;
    }
    if (Weather_TickDays()) {
      weather = VIEW->defaultWeather;
    }
  }
  return weather;
}
