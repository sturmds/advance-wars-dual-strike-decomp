#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when the owner of the unit slot (slots come in blocks of 64 per
 * player, player 1 first) owns tile (x, y). */
BOOL UnitSlot_OwnsTile(int slot, int x, int y) {
  return (slot >> 6) + 1 ==
         STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]] >> 5;
}
