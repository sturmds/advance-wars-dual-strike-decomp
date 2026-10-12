#include "types.h"
#include "game/weather.h"

void Sound_PlaySe(int se);

extern const WeatherInfo gWeatherInfo[];

/* Plays the start sound of the task's weather. The signed view of weather is
 * needed to match (ldrsh). */
void WeatherTask_PlaySound(WeatherTaskWork* work) {
  Sound_PlaySe(gWeatherInfo[work->weather].sound);
}
