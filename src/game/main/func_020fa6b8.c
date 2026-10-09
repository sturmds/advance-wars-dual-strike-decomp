#include "types.h"
#include "game/weather.h"

void Gfx_RequestLayerCopy(int weather);

void func_020fa6b8(void) {
    Gfx_RequestLayerCopy(WEATHER_CLEAR);
}
