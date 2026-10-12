#include "types.h"
#include "game/design_map.h"

/* Whether the row describes a map (playerCount 0xff marks an unused row). */
int DesignMap_IsUsed(const DesignMap* row) {
  return (row->playerCount != 255) ? 1 : 0;
}
