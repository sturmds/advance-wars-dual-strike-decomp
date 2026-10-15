#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

/* Copies MAP_UNITS_PER_PLAYER units from src into the unit slots of player. */
void Units_CopyBlockToPlayer(MapUnit* src, int player) {
  MapUnit* dst = &MAP_STATE->units[(player - 1) * MAP_UNITS_PER_PLAYER];
  int i;

  for (i = 0; i < MAP_UNITS_PER_PLAYER; i++) {
    *dst++ = *src++;
  }
}
