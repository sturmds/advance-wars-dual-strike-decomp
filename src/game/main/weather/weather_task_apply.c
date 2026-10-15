#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Task callback in the weather script: makes the task's weather the map's
 * current weather and runs the screen config's unk_58 hook with it and the
 * tileset. */
void WeatherTask_Apply(WeatherTaskWork* work) {
  MAP_STATE->unk_1c4->weather = work->weather;
  if (HOOKS->unk_58 != NULL) {
    HOOKS->unk_58(MAP_STATE->unk_1c4->weather, MAP_STATE->unk_1c4->tileset);
  }
}
