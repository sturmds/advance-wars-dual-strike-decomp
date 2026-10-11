#include "types.h"
#include "game/design_map.h"

void func_020d70ec(void *map, DesignMapRecord *record);
void func_020d727c(DesignMapRecord *record);
void Map_ConvertCellsToTiles(void);

/* Unpacks a design map record into map (func_020d70ec), loads it into the
 * game state (func_020d727c) and converts its cells to tiles. */
void DesignMap_ApplyRecord(void *map, DesignMapRecord *record)
{
    func_020d70ec(map, record);
    func_020d727c(record);
    Map_ConvertCellsToTiles();
}
