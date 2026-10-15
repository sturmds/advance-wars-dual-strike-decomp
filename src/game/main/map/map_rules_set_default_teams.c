#include "types.h"
#include "game/Unk_02080440.h"

/* Puts players 1-4 on teams 0-3. */
void MapRules_SetDefaultTeams(void) {
  MAP_STATE->team[1] = 0;
  MAP_STATE->team[2] = 1;
  MAP_STATE->team[3] = 2;
  MAP_STATE->team[4] = 3;
}
