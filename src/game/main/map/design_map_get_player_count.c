#include "types.h"
#include "game/design_map.h"

extern DesignMap data_022a6c84[];

/* Returns the player count of a player-made map; index 0 is map id MAP_DESIGN_FIRST. */
u8 DesignMap_GetPlayerCount(u8 index) {
  return data_022a6c84[index].playerCount;
}
