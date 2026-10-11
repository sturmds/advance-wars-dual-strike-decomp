#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"

int Ov006_TileMatches(int x, int y, int terrain);
void Ov006_StoreMapCell(int x, int y, int cell);
int func_ov006_023670ec(int x, int y);
int Ov006_GetShoalGraphic(int x, int y);
void func_ov006_0235ddac(int x, int y);
int Ov006_GetWaterCell(int x, int y);
int func_ov006_02351238(int x, int y);

extern Unk_02080440 *data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

#define CELL_PLAIN 1
/* Plain cells that the code turns back into CELL_PLAIN once the tile below is
 * no longer a mountain: likely plain with the top of that mountain drawn on it. */
#define CELL_PLAIN_ABOVE_MOUNTAIN   3
#define CELL_PLAIN_ABOVE_MOUNTAIN_2 67

/* Picks the cell of tile x, y again from its terrain and its neighbours:
 * road, shoal (removed by func_ov006_0235ddac when no shoal cell fits),
 * plain above a former mountain, and sea, then the final pass of
 * func_ov006_02351238 for every terrain. */
void Ov006_RefitTileCell(int x, int y)
{
    int cell;

    if (Ov006_TileMatches(x, y, TERRAIN_ROAD)) {
        Ov006_StoreMapCell(x, y, func_ov006_023670ec(x, y));
    }
    if (Ov006_TileMatches(x, y, TERRAIN_SHOAL)) {
        cell = Ov006_GetShoalGraphic(x, y);
        if (cell < 0) {
            func_ov006_0235ddac(x, y);
        } else {
            Ov006_StoreMapCell(x, y, cell);
        }
    }
    if (Ov006_TileMatches(x, y, TERRAIN_PLAIN)) {
        cell = CELL_AT(x, y);
        if ((cell == CELL_PLAIN_ABOVE_MOUNTAIN_2 || cell == CELL_PLAIN_ABOVE_MOUNTAIN)
            && !Ov006_TileMatches(x, y + 1, TERRAIN_MOUNTAIN)) {
            Ov006_StoreMapCell(x, y, CELL_PLAIN);
        }
    }
    if (Ov006_TileMatches(x, y, TERRAIN_SEA)) {
        Ov006_StoreMapCell(x, y, Ov006_GetWaterCell(x, y));
    }
    Ov006_StoreMapCell(x, y, func_ov006_02351238(x, y));
}
