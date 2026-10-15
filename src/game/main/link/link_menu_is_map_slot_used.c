#include "types.h"
#include "game/menu_state.h"

int DesignMap_IsUsed(DesignMap* row);

/* Whether the map slot (0-2) of a link player holds a map. */
int LinkMenu_IsMapSlotUsed(int player, int slot) {
  return DesignMap_IsUsed(
      &LINK_MAP_ROWS[player * LINK_DESIGN_MAPS_PER_PLAYER + slot]);
}
