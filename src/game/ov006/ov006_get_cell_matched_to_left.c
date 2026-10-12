#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Ov006_MatchCellToLeftTile(int x, int y, int mapCell);

/* The cell code at tile x, y, switched by Ov006_MatchCellToLeftTile to the
 * form that fits the tile on its left. */
int Ov006_GetCellMatchedToLeft(int x, int y) {
  MapTiles* map = data_027c027c->unk_48[data_027c0284]->mapTiles;

  return Ov006_MatchCellToLeftTile(x, y, map->mapCells[map->rowStart[y] + x]);
}
