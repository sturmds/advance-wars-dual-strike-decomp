#include "types.h"
#include "game/weather.h"

void Screen_SetBothNibbles(int weather);

void func_ov006_02350eb0(void) { Screen_SetBothNibbles(WEATHER_CLEAR); }
