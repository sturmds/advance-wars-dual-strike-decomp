#include "types.h"
#include "game/weather.h"

void Weather_SetDays(int weather);
int Weather_ResolvePowerSnow(int weather);
WeatherTaskWork *func_02017698(const void *descriptor, int arg); /* starts a task from descriptor, returns its work block */

extern const u32 data_02166d18[];

/* Starts the weather task for a weather value and sets how long it lasts. */
void Weather_Set(int weather)
{
    WeatherTaskWork *work;

    Weather_SetDays(weather);
    work = func_02017698(data_02166d18, 0);
    work->weather = Weather_ResolvePowerSnow(weather);
}
