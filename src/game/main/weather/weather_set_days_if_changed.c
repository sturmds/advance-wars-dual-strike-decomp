#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

void Weather_SetDays(int weather);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Weather_SetDays, except for clear weather and for the weather already in
 * effect. Power snow always sets the days. */
void Weather_SetDaysIfChanged(int weather) {
  if (weather != WEATHER_POWER_SNOW) {
    if (weather == WEATHER_CLEAR) {
      return;
    }
    if (weather == VIEW->weather) {
      return;
    }
  }
  Weather_SetDays(weather);
}
