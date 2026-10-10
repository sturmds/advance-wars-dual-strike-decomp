#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* Guess: tile byte (terrain and owner) is a city DeployCity accepts, or terrain 8 owned by state->unk_04. */
BOOL func_020f4a94(u8 tile);

/* TRUE when no unit stands on tile (x, y) and func_020f4a94 accepts its terrain
 * and owner. The tile byte is read before the unit test to match. */
BOOL Map_IsVacantDeployTile(int x, int y)
{
    MapTiles *mapTiles = data_027c027c->unk_48[data_027c0284]->mapTiles;
    int index = x + mapTiles->rowStart[y];
    u8 unit = mapTiles->unitAt[index];
    u8 tile = mapTiles->tiles[index];

    if (unit != 0) {
        return FALSE;
    }
    return func_020f4a94(tile) != 0;
}
