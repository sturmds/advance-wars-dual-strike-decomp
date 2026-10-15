#include "types.h"
#include "game/Unk_02080440.h"

/* Unit menu entry test: 1 hides the entry. Wait is hidden while another unit
 * stands on the destination tile. */
s8 UnitMenu_WaitHidden(void) {
  return MAP_STATE->mapTiles->unitAt
             [MAP_STATE->actingUnitDestX +
              MAP_STATE->mapTiles->rowStart[MAP_STATE->actingUnitDestY]] != 0;
}
