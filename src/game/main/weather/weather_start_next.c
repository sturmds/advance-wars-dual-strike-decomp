#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"
#include "game/script_slot.h"

void Weather_SetDaysIfChanged(int weather);
WeatherTaskWork* ScriptSlot_Start(const void* descriptor, int arg);

extern ScriptCommand data_02166d18[];

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Sets the days for the map's next weather and, when it differs from the
 * current one, starts the weather task for it (as Weather_Set does). The
 * current weather field itself is not written here. */
void Weather_StartNext(void) {
  WeatherTaskWork* work;

  Weather_SetDaysIfChanged(VIEW->nextWeather);
  if (VIEW->weather == VIEW->nextWeather) {
    return;
  }
  work = ScriptSlot_Start(data_02166d18, 0);
  work->weather = VIEW->nextWeather;
}
