#include "types.h"
#include "game/map_unit.h"

typedef struct TilePos {
  s16 x;
  s16 y;
} TilePos;

BOOL Map_CanSendToOtherMapFrom(MapUnit* unit, int x, int y);
BOOL Unit_LacksOtherMapSlots(MapUnit* unit);
BOOL Unit_FindOtherMapTile(MapUnit* unit, TilePos* tile);

/* Name is a guess from the one caller, AiUnit_MoveToOtherMap, which asks
 * it about each reachable tile. TRUE when Map_CanSendToOtherMapFrom accepts the tile,
 * Unit_LacksOtherMapSlots does not return 1 for it, and Unit_FindOtherMapTile accepts the tile. */
BOOL AiUnit_CanUseOtherMapFrom(MapUnit* unit, s16 x, s16 y) {
  TilePos tile;

  if (!Map_CanSendToOtherMapFrom(unit, x, y)) {
    return FALSE;
  }
  if (Unit_LacksOtherMapSlots(unit) == 1) {
    return FALSE;
  }
  tile.x = x;
  tile.y = y;
  return Unit_FindOtherMapTile(unit, &tile) != 0;
}
