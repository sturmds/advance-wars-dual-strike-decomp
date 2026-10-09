#include "types.h"
#include "game/weather.h"

void Map_SetCoSelectPhase(int weather);

void func_020e889c(void) {
    Map_SetCoSelectPhase(WEATHER_CLEAR);
}
