#ifndef GAME_DESIGN_MAP_H
#define GAME_DESIGN_MAP_H

#include "types.h"

/* One row of data_022a6c84, 27 bytes. Row n holds map id
 * MAP_DESIGN_FIRST + n. */
typedef struct DesignMap {
    u8 pad_00[2];
    char name[0x11];                   /* from offset 2 */
    u8 playerCount;                    /* 0xff: row unused */
    u8 armyColours[5];                 /* per slot, like MapHeader +0x88 */
    u8 unk_19;
    u8 unk_1a;
} DesignMap;

typedef char DesignMap_size_check[sizeof(DesignMap) == 0x1b ? 1 : -1];

/* A player-made map as Map_LoadData reads it (Save_LoadRecord, 0x724 bytes):
 * the map data (game/map_data.h) first. */
typedef struct DesignMapRecord {
    u8 width;                          /* like MapData */
    u8 height;
    u16 tileCodes[600];                /* MapTiles.mapCells, row by row */
    char name[0x10];
    u8 unk_4c2;                        /* DesignMap_BuildRecord clears it */
    u8 playerCount;                    /* 0xff or 0: no map in this record */
    u8 armyColours[5];                 /* per slot, like DesignMap.armyColours */
    u8 propertyCount;                  /* urban tiles (Ov000_gUrbanTerrain) on the map */
    u8 unk_4ca;                        /* 1 + the most urban tiles one owner has, the neutral slot included */
    u8 units[600];                     /* per tile: 0, or the unit type with an owner code in bits 6-7 (DesignMap_BuildRecord) */
    u8 pad_723[0x724 - 0x723];
} DesignMapRecord;

typedef char DesignMapRecord_size_check[sizeof(DesignMapRecord) == 0x724 ? 1 : -1];

#endif
