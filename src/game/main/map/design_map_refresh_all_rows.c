#include "types.h"
#include "game/map_header.h"

int DesignMap_RefreshRow(u32 slot);

/* Refreshes every row of the design map list. */
void DesignMap_RefreshAllRows(void) {
  u8 slot;

  for (slot = 0; slot < MAP_CUSTOM_COUNT; slot++) {
    DesignMap_RefreshRow(slot);
  }
}
