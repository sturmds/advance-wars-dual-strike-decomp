#include "types.h"
#include "game/weather.h"

void Weather_StartEffect(int weather);

void func_020cce88(void) {
    Weather_StartEffect(WEATHER_CLEAR);
}
