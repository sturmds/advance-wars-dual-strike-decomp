#include "types.h"
#include "game/weather.h"

void Weather_Set(int weather);
void func_020d5684(void);

/* Olaf's CO Power: snow, with no damage. */
void Blizzard(void)
{
    Weather_Set(WEATHER_POWER_SNOW);
    func_020d5684();
}
