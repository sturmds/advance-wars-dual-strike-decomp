#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void RangeMap_SetBuffer(void *tileMap);

/* Sets every tile of a per-tile map to value, without touching rangeMap.
 * Afterwards rangeMap points at MapTiles.rangeMapTiles again. */
void TileMap_Fill(void *tileMap, u8 value)
{
    u8 *rows[40];
    int x;
    int y;

    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        rows[y] = (u8 *)tileMap + STATE->mapTiles->rowStart[y];
    }
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            rows[y][x] = value;
        }
    }
    RangeMap_SetBuffer(STATE->mapTiles->rangeMapTiles);
}
