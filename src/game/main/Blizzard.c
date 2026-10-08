#include "types.h"
#include "game/weather.h"

void func_020d2aa8(int weather);
void func_020d5684(void);

/* Olaf's CO Power: snow, with no damage. */
void Blizzard(void)
{
    func_020d2aa8(WEATHER_POWER_SNOW);
    func_020d5684();
}
