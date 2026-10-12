#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

/* Weather id for a value passed to Weather_Set: power snow, which lasts
 * two days, becomes plain snow. */
int Weather_ResolvePowerSnow(int weather) {
  if (weather == WEATHER_POWER_SNOW) {
    weather = WEATHER_SNOW;
  }
  return weather;
}
