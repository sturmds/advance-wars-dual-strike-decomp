#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 data_022a6b34[];
extern Unk_02080440_48 data_02189cd8;

s8 UnitMenu_JoinHidden(void);
s8 UnitMenu_BoardHidden(void);
BOOL Map_CanSendToOtherMapFrom(MapUnit* unit, int x, int y);
BOOL Unit_LacksOtherMapSlots(MapUnit* unit);
BOOL Unit_FindOtherMapTile(MapUnit* unit, TilePoint* pos);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test for the first Send entry (table at 0x021661d0), which
 * sends the acting unit to the other map of a two-map battle. 0 shows the
 * entry, 1 hides it. It shows when nothing is pending in data_022a6b34, this
 * is the first map, the unit can neither join nor board here, the battle
 * allows a send from the destination tile, and the other map has a free
 * unit slot and a tile for the unit. */
s8 UnitMenu_SendHidden(void) {
  s16 x = STATE->actingUnitDestX;
  s16 y = STATE->actingUnitDestY;
  TilePoint tile;

  if (data_022a6b34[0] + data_022a6b34[1] != 0) {
    return 1;
  }
  if (STATE == &data_02189cd8) { /* the second map's state */
    return 1;
  }
  if (!UnitMenu_JoinHidden()) {
    return 1;
  }
  if (!UnitMenu_BoardHidden()) {
    return 1;
  }
  if (!Map_CanSendToOtherMapFrom(STATE->actingUnit, x, y)) {
    return 1;
  }
  if (Unit_LacksOtherMapSlots(STATE->actingUnit) == 1) {
    return 1;
  }
  tile.x = x;
  tile.y = y;
  return Unit_FindOtherMapTile(STATE->actingUnit, &tile) == 0;
}
