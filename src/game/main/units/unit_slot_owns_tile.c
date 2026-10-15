#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when the owner of the unit slot (slots come in blocks of 64 per
 * player, player 1 first) owns tile (x, y). */
BOOL UnitSlot_OwnsTile(int slot, int x, int y) {
  return (slot >> 6) + 1 ==
         MAP_STATE->mapTiles->tiles[x + MAP_STATE->mapTiles->rowStart[y]] >> 5;
}
