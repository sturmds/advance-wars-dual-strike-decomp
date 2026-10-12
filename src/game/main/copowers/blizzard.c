#include "types.h"
#include "game/weather.h"

void Weather_Set(int weather);
void Power_ResetScrollAndRefresh(void);

/* Olaf's CO Power: snow, with no damage. */
void Blizzard(void) {
  Weather_Set(WEATHER_POWER_SNOW);
  Power_ResetScrollAndRefresh();
}
