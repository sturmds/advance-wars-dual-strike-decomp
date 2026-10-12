#include "types.h"
#include "game/weather.h"

void IrqSlot_CallAndClear(int weather);

void func_0210cc10(void) { IrqSlot_CallAndClear(WEATHER_CLEAR); }
