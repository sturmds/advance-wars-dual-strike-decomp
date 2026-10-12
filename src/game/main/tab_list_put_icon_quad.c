#include "types.h"

void Bg_PutTileQuad(int x, int y, int firstTile, int palette);
int ArmyStyle_GetIconTile(
    int player,
    int index); /* tile offset of icon number index in the set of a player */
int UnitIcons_GetTileCount(void);

/* Draws icon number index of a player's set as a tile quad at tile (x, y). */
void TabList_PutIconQuad(int x, int y, int index, int player, int palette) {
  int tile =
      996 - UnitIcons_GetTileCount() + ArmyStyle_GetIconTile(player, index);

  Bg_PutTileQuad(x, y, tile, palette);
}
