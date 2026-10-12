#include "types.h"
#include "game/map_header.h"
#include "game/design_map.h"

extern u8 gArmyColours[ARMY_SLOT_COUNT];
extern DesignMap data_022a6c84[];

/* Copies the army colour per slot of map mapId into gArmyColours. Design
 * maps come from data_022a6c84 instead of the header table. */
void Map_LoadArmyColoursById(u32 mapId) {
  int slot;

  if (mapId >= MAP_DESIGN_FIRST && mapId < MAP_DESIGN_END) {
    for (slot = 0; slot < ARMY_SLOT_COUNT; slot++) {
      /* needed to match: index from mapId, then step back */
      gArmyColours[slot] =
          (data_022a6c84 + mapId)[-MAP_DESIGN_FIRST].armyColours[slot];
    }
  } else {
    for (slot = 0; slot < ARMY_SLOT_COUNT; slot++) {
      gArmyColours[slot] = Ov000_gMapHeaders[mapId].army[slot];
    }
  }
}
