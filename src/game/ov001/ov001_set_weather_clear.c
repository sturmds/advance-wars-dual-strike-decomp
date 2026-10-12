#include "types.h"
#include "game/weather.h"

/* Writes `weather` into the current map state's weather bytes, then calls
 * Weather_Set. */
void Weather_Change(int weather);

void Ov001_SetWeatherClear(void) { Weather_Change(WEATHER_CLEAR); }
