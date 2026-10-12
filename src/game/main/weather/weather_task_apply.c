#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])
#define HOOKS (data_027c027c->unk_50[data_027c0284])

/* Task callback in the weather script: makes the task's weather the map's
 * current weather and runs the screen config's unk_58 hook with it and the
 * tileset. */
void WeatherTask_Apply(WeatherTaskWork* work) {
  GAME->unk_1c4->weather = work->weather;
  if (HOOKS->unk_58 != NULL) {
    HOOKS->unk_58(GAME->unk_1c4->weather, GAME->unk_1c4->tileset);
  }
}
