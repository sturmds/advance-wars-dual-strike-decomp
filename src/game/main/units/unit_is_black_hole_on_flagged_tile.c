#include "types.h"
#include "game/Unk_02080440.h"

/* MAP_STATE->tileHp bit 7 of a tile; meaning unknown. */
#define TILE_HP_FLAG 0x80

/* TRUE when the unit belongs to the Black Hole army and tile (x, y) has the
 * tileHp flag set. Callers pass the acting unit's own tile. */
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y) {
  if (MAP_STATE->players[MAP_UNIT_OWNER(MAP_STATE, unit)].armyColour !=
      ARMY_BLACK_HOLE) {
    return FALSE;
  }
  return (MAP_STATE->tileHp[x + MAP_STATE->mapTiles->rowStart[y]] &
          TILE_HP_FLAG) != 0;
}
