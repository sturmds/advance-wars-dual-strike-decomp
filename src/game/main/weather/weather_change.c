#include "types.h"
#include "game/Unk_02080440.h"

void Weather_Set(int weather);

/* Makes `weather` the map's current and default weather and starts it. randomWeather
 * (the menu "weather setting 0" flag) is cleared. */
void Weather_Change(int weather) {
  data_027c027c->unk_48[data_027c0284]->unk_1c4->defaultWeather = weather;
  data_027c027c->unk_48[data_027c0284]->unk_1c4->weather = weather;
  data_027c027c->unk_48[data_027c0284]->unk_1c4->nextWeather = weather;
  data_027c027c->unk_48[data_027c0284]->unk_1c4->randomWeather = 0;
  Weather_Set(weather);
}
