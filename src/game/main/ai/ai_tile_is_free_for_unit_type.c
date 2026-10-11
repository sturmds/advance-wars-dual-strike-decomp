#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define CHART ((const s8 (*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

BOOL func_020b6c84(int x, int y, int unitType)
{
    int index;
    Unk_02080440_48 **slot = &data_027c027c->unk_48[data_027c0284];
    MapTiles *map = (*slot)->mapTiles;

    index = x + map->rowStart[y];

    if (map->unitAt[index] != 0) {
        return FALSE;
    }
    return CHART[Ov000_gUnitStats[unitType].moveType][MAP_TILE_TERRAIN(map->tiles[index])] >= 0;
}
