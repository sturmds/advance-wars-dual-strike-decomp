#include "types.h"

u8 Map_GetPlayerCount(u16 mapId);

/* Nonzero when the map has players: player-made map slots that are unused
 * report a count of 0xff or 0. */
int Map_Exists(u16 mapId) {
  u8 count = Map_GetPlayerCount(mapId);

  if (count == 0xff || count == 0) {
    return 0;
  }
  return 1;
}
