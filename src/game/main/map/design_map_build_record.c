#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map.h"
#include "game/terrain.h"

void String_Copy(char *dest, const char *src);

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern s8 Ov000_gUrbanTerrain[32]; /* nonzero for urban terrain, indexed by terrain id */

#define GAME (data_027c027c->unk_48[data_027c0284])
#define TILE_INDEX(x, y) ((x) + GAME->mapTiles->rowStart[y])

/* Writes the loaded map into a design map record: size, name, the cell code
 * and unit of each tile, urban tile counts and the army colours. With
 * countHqs set to 1 the player count is the number of HQ tiles, else 0. */
void DesignMap_BuildRecord(const char *name, DesignMapRecord *record, int countHqs)
{
    u8 urbanCounts[6];
    u16 hqs;
    int x;
    u8 terrainTile;
    u8 ownerCodes[6];
    int y;
    u16 tile;
    u16 i;
    u8 unitAt;
    Unk_02080440_48 *state;

    String_Copy(record->name, name);
    record->unk_4c2 = 0;
    record->width = GAME->unk_1c4->mapWidth;
    record->height = GAME->unk_1c4->mapHeight;
    for (i = 0; i < 5; i++) {
        record->armyColours[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        urbanCounts[i] = 0;
    }
    for (i = 0; i < 5; i++) {
        ownerCodes[GAME->army[i]] = (i - 1) << 6;
    }

    tile = 0;
    hqs = 0;
    for (y = 0; y < GAME->unk_1c4->mapHeight; y++) {
        state = GAME;
        for (x = 0; x < GAME->unk_1c4->mapWidth; x++) {
            unitAt = state->mapTiles->unitAt[x + state->mapTiles->rowStart[y]];
            if (unitAt != 0) {
                record->units[tile] = state->units[unitAt].type | ownerCodes[(unitAt >> 6) + 1];
            } else {
                record->units[tile] = 0;
            }
            record->tileCodes[tile] = GAME->mapTiles->mapCells[TILE_INDEX(x, y)];
            tile++;
            state = GAME;
            terrainTile = state->mapTiles->tiles[x + state->mapTiles->rowStart[y]];
            if (Ov000_gUrbanTerrain[MAP_TILE_TERRAIN(terrainTile)] != 0) {
                urbanCounts[5]++;
                urbanCounts[terrainTile >> 5]++;
            }
            if (MAP_TILE_TERRAIN(terrainTile) == TERRAIN_HQ) {
                hqs++;
            }
        }
    }

    for (i = 0; i < 5; i++) {
        if (urbanCounts[0] < urbanCounts[i]) {
            urbanCounts[0] = urbanCounts[i];
        }
    }
    record->propertyCount = urbanCounts[5];
    record->unk_4ca = urbanCounts[0] + 1;
    if (countHqs == 1) {
        record->playerCount = hqs;
    } else {
        record->playerCount = 0;
    }
    for (i = 0; i < 5; i++) {
        record->armyColours[i] = GAME->army[i];
    }
}
