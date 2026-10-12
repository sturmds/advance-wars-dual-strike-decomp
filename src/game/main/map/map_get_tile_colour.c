#include "types.h"
#include "game/terrain.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Returns the army colour (the palette, 1 Orange Star ... 5 Black Hole) of
 * the player who owns a map tile, 0 for an unowned tile. */
u8 Map_GetTileColour(int tile) {
  if ((tile & 0xe0) == 0) {
    return 0;
  }
  return STATE->players[MAP_TILE_OWNER(tile)].armyColour;
}
