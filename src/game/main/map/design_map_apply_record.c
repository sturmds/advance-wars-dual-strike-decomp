#include "types.h"
#include "game/design_map.h"

void DesignMap_UnpackRecord(void *map, DesignMapRecord *record);
void func_020d727c(DesignMapRecord *record);
void Map_ConvertCellsToTiles(void);

/* Unpacks a design map record into map (DesignMap_UnpackRecord), loads it into the
 * game state (func_020d727c) and converts its cells to tiles. */
void DesignMap_ApplyRecord(void *map, DesignMapRecord *record)
{
    DesignMap_UnpackRecord(map, record);
    func_020d727c(record);
    Map_ConvertCellsToTiles();
}
