#include "types.h"
#include "game/Unk_02080440.h"

void WeatherFx_UpdateSnow(void);
void WeatherFx_UpdateRain(void);
void WeatherFx_UpdateSand(void);

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Updates the particles of the current map's weather. Clear weather has none;
 * its empty case is needed to match (it makes the switch a jump table). */
void WeatherFx_Update(void) {
  switch (VIEW->weather) {
    case WEATHER_CLEAR:
      break;
    case WEATHER_SNOW:
      WeatherFx_UpdateSnow();
      break;
    case WEATHER_RAIN:
      WeatherFx_UpdateRain();
      break;
    case WEATHER_SAND:
      WeatherFx_UpdateSand();
      break;
  }
}
