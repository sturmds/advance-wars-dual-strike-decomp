#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_action.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern UnitAction data_0229d8c8;

void Link_SendPacket(void* payload);

#define WEATHER (data_027c027c->unk_5c[data_027c0284])

/* Link play: sends the next weather and the per-player weather days (action 13). */
void Link_SendWeather(int nextWeather) {
  int i;

  data_0229d8c8.action = 13;
  data_0229d8c8.target = nextWeather;
  data_0229d8c8.unk_07 = WEATHER->sandChance;
  data_0229d8c8.packed[0] = WEATHER->rainChance;
  data_0229d8c8.packed[1] = WEATHER->snowChance;
  for (i = 0; i < PLAYER_SLOT_LAST; i++) {
    data_0229d8c8.packed[2 + i] = WEATHER->weatherDays[1 + i];
  }
  Link_SendPacket(&data_0229d8c8);
}
