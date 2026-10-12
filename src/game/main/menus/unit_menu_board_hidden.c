#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Tile_HasTransportToBoard(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test: 1 hides the Board entry. It shows when the acting
 * unit's destination tile holds a transport it can board. */
s8 UnitMenu_BoardHidden(void) {
  return Tile_HasTransportToBoard(STATE->actingUnitDestX,
                                  STATE->actingUnitDestY) != 1;
}
