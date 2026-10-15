#include "types.h"
#include "game/Unk_02080440.h"
#include "game/tile_point.h"

extern u8 data_022a6b34[];
extern Unk_02080440_48 data_02189cd8;

s8 UnitMenu_JoinHidden(void);
s8 UnitMenu_BoardHidden(void);
BOOL Map_CanSendToOtherMapFrom(MapUnit* unit, int x, int y);
BOOL Unit_LacksOtherMapSlots(MapUnit* unit);
BOOL Unit_FindOtherMapTile(MapUnit* unit, TilePoint* pos);

/* Unit menu entry test for the second Send entry (table at 0x021661f0), which
 * sends the acting unit to the other map of a two-map battle. 1 hides the
 * entry. It shows only when the send passes every test of
 * UnitMenu_SendHidden except the last two: the other map has no free unit
 * slot or no tile for the unit. Guess: 2 shows the entry greyed out. */
s8 UnitMenu_SendBlockedHidden(void) {
  s16 x = MAP_STATE->actingUnitDestX;
  s16 y = MAP_STATE->actingUnitDestY;
  TilePoint tile;

  if (data_022a6b34[0] + data_022a6b34[1] != 0) {
    return 1;
  }
  if (MAP_STATE == &data_02189cd8) { /* the second map's state */
    return 1;
  }
  if (!UnitMenu_JoinHidden()) {
    return 1;
  }
  if (!UnitMenu_BoardHidden()) {
    return 1;
  }
  if (!Map_CanSendToOtherMapFrom(MAP_STATE->actingUnit, x, y)) {
    return 1;
  }
  if (Unit_LacksOtherMapSlots(MAP_STATE->actingUnit) == 1) {
    return 2;
  }
  tile.x = x;
  tile.y = y;
  if (Unit_FindOtherMapTile(MAP_STATE->actingUnit, &tile) == 0) {
    return 2;
  } else {
    return 1;
  }
}
