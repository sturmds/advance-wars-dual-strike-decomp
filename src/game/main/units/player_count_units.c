#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Counts the used unit slots of a player (1-4). Only slots 1 to 50 of its
 * block of 64 are looked at. */
int Player_CountUnits(int player) {
  int count = 0;
  int i;
  int slot = (player - 1) * MAP_UNITS_PER_PLAYER + 1;
  MapUnit* units = data_027c027c->unk_48[data_027c0284]->units;

  for (i = 1; i < MAP_UNITS_MAX + 1; i++) {
    if (units[slot++].type != 0) {
      count++;
    }
  }
  return count;
}
