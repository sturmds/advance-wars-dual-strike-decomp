#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Puts players 1-4 on teams 0-3. */
void MapRules_SetDefaultTeams(void) {
  STATE->team[1] = 0;
  STATE->team[2] = 1;
  STATE->team[3] = 2;
  STATE->team[4] = 3;
}
