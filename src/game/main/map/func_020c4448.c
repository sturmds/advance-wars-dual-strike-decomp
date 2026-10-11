#include "types.h"
#include "game/Unk_02080440.h"
#include "game/range_map.h"
#include "game/terrain.h"

void Map_MarkCrystalObeliskRange(void);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s8 Ov000_gUrbanTerrain[32];
extern s8 data_ov000_022f4578[32];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Scans the map once for property tiles (urban terrain, plus silos): counts them
 * per owner in Player.propertyCount, numbers them in MapTiles.unk_1928 and lists them in
 * unk_6088. A second scan lists up to 92 tiles in unk_608c. Register allocation
 * does not match yet. */
void func_020c4448(void)
{
    int listed;
    int y;
    int count;
    int col;
    u32 urban;
    int i;
    int x;
    int row;
    int tile;

    count = 0;
    RangeMap_SetBuffer(&STATE->mapTiles->unk_1928);
    RangeMap_Fill(-1);
    for (i = 0; i < 5; i++) {
        STATE->players[i].propertyCount = 0;
    }
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
            tile = STATE->mapTiles->tiles[x + STATE->mapTiles->rowStart[y]];
            urban = Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(tile)];
            if (urban != 0) {
                STATE->players[MAP_TILE_OWNER(tile)].propertyCount++;
            }
            if (urban != 0 || MAP_TILE_TERRAIN(tile) == TERRAIN_SILO) {
                STATE->mapTiles->unk_1928[x + STATE->mapTiles->rowStart[y]] = count;
                STATE->unk_6088[count].terrain = MAP_TILE_TERRAIN(tile);
                STATE->unk_6088[count].x = x;
                STATE->unk_6088[count].y = y;
                STATE->unk_6088[count].claimCount = 0;
                count++;
            }
        }
    }
    STATE->unk_6088[count].terrain = 0xff;
    listed = 0;
    for (row = 0; row < STATE->unk_1c4->mapHeight; row++) {
        for (col = 0; col < STATE->unk_1c4->mapWidth; col++) {
            tile = STATE->mapTiles->tiles[col + STATE->mapTiles->rowStart[row]];
            if (data_ov000_022f4578[MAP_TILE_TERRAIN(tile)] != 0 || (STATE->gameMode == 4 && MAP_TILE_TERRAIN(tile) == 16)) {
                if (listed < 92) {
                    STATE->unk_608c[listed].tile = tile;
                    STATE->unk_608c[listed].x = col;
                    STATE->unk_608c[listed].y = row;
                }
                listed++;
            }
        }
    }
    if (listed < 92) {
        STATE->unk_608c[listed].tile = 0xff;
    }
    Map_MarkCrystalObeliskRange();
}
