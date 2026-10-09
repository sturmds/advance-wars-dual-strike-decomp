#include "types.h"
#include "game/weather.h"

/* Writes `weather` into the current map state's weather bytes, then calls
 * Weather_Set. */
void func_020d207c(int weather);

void Ov001_SetWeatherClear(void) {
    func_020d207c(WEATHER_CLEAR);
}
