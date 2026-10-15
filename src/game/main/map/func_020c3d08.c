#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_header.h"

int Map_GetPlayerCount(
    u32 mapId); /* needed to match: the result is not taken as u8 */

/* Player count of a map, adjusted by game mode: one less in mode 0, 1 in
 * modes 1 and 3, and not at all on custom maps. Map_SetupPlayerCos compares
 * the player number against it in Hard Campaign. */
u8 func_020c3d08(u32 mapId) {
  int count = Map_GetPlayerCount(mapId);

  if (MAP_IS_CUSTOM(mapId)) {
    return count;
  }
  switch (data_027c027c->unk_48[data_027c0284]->gameMode) {
    case 0:
      return count - 1;
    case 1:
    case 3:
      return 1;
    default:
      return count;
  }
}
