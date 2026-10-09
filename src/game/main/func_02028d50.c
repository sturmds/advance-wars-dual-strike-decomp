#include "types.h"
#include "game/weather.h"

void Screen_SetBgMode(int weather);

void func_02028d50(void) {
    Screen_SetBgMode(WEATHER_CLEAR);
}
