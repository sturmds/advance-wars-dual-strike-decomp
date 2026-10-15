#include "types.h"
#include "game/Unk_02080440.h"

/* Makes `weather` the map's current and default weather without starting
 * it (Weather_Change does). randomWeather is cleared. */
void SetMapWeather(int weather) {
  data_027c027c->unk_48[data_027c0284]->unk_1c4->defaultWeather = weather;
  data_027c027c->unk_48[data_027c0284]->unk_1c4->weather = weather;
  data_027c027c->unk_48[data_027c0284]->unk_1c4->nextWeather = weather;
  data_027c027c->unk_48[data_027c0284]->unk_1c4->randomWeather = 0;
}
