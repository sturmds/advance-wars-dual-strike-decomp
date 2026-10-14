#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define WEATHER (data_027c027c->unk_5c[data_027c0284])

void func_020d2190(void);

/* Clears every player's weather days and sets the snow, rain and sand chances
 * (percent, for Weather_PickNext) to 6 minus the number of players that have a
 * controller. */
void Weather_ResetChances(void) {
  int player;
  int chance2;
  int chance1;
  int chance0;
  chance2 = 6;
  chance1 = 6;
  chance0 = 6;

  for (player = 1; player <= PLAYER_SLOT_LAST; player++) {
    if (STATE->players[player].controller != PLAYER_CONTROLLER_NONE) {
      chance1--;
      chance2--;
      chance0--;
    }
    WEATHER->weatherDays[player] = 0;
  }
  WEATHER->snowChance = chance2;
  WEATHER->rainChance = chance1;
  WEATHER->sandChance = chance0;
  func_020d2190();
}
