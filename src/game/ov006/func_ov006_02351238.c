#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

int Ov006_MatchCellToLeftTile(int x, int y, int mapCell);

/* Looks up the map cell code at tile x, y and passes it on with the tile. */
int func_ov006_02351238(int x, int y)
{
    MapTiles *map = data_027c027c->unk_48[data_027c0284]->mapTiles;

    return Ov006_MatchCellToLeftTile(x, y, map->mapCells[map->rowStart[y] + x]);
}
