#include "types.h"
#include "game/Unk_02080440.h"

BOOL Tile_HasTransportToBoard(int x, int y);

/* Unit menu entry test: 1 hides the Board entry. It shows when the acting
 * unit's destination tile holds a transport it can board. */
s8 UnitMenu_BoardHidden(void) {
  return Tile_HasTransportToBoard(MAP_STATE->actingUnitDestX,
                                  MAP_STATE->actingUnitDestY) != 1;
}
