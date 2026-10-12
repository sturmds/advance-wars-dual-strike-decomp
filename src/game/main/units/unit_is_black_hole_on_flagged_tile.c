#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* STATE->tileHp bit 7 of a tile; meaning unknown. */
#define TILE_HP_FLAG 0x80

/* TRUE when the unit belongs to the Black Hole army and tile (x, y) has the
 * tileHp flag set. Callers pass the acting unit's own tile. */
BOOL Unit_IsBlackHoleOnFlaggedTile(MapUnit* unit, int x, int y) {
  if (STATE->players[MAP_UNIT_OWNER(STATE, unit)].armyColour !=
      ARMY_BLACK_HOLE) {
    return FALSE;
  }
  return (STATE->tileHp[x + STATE->mapTiles->rowStart[y]] & TILE_HP_FLAG) != 0;
}
