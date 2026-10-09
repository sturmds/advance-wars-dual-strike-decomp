#include "types.h"
#include "game/weather.h"

void Gfx_RequestLayerCopy(int weather);

void func_0200a6c0(void) {
    Gfx_RequestLayerCopy(WEATHER_CLEAR);
}
