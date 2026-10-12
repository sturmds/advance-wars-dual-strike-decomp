#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Copies MAP_UNITS_PER_PLAYER units from src into the unit slots of player. */
void Units_CopyBlockToPlayer(MapUnit* src, int player) {
  MapUnit* dst = &STATE->units[(player - 1) * MAP_UNITS_PER_PLAYER];
  int i;

  for (i = 0; i < MAP_UNITS_PER_PLAYER; i++) {
    *dst++ = *src++;
  }
}
