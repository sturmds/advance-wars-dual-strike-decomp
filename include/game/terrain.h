#ifndef GAME_TERRAIN_H
#define GAME_TERRAIN_H

#include "types.h"
#include "game/text_ref.h"

/* Terrain ids, from romhacking.net doc 890 (RadioTails). Add more as code
 * uses them. */
enum TerrainId {
    TERRAIN_NONE,
    TERRAIN_PLAIN,
    TERRAIN_RIVER,
    TERRAIN_MOUNTAIN,
    TERRAIN_WOOD,
    TERRAIN_ROAD,
    TERRAIN_CITY,
    TERRAIN_SEA
};

/* Fixed stats of one terrain id, 0x18 bytes, in ov000 at 0x022f4ccc.
 * The table holds terrain ids 0-30; Ov000_gUnitStats starts right
 * after. */
typedef struct TerrainStats {
    s32 defenseStars;                  /* plain 1, wood 2, city 3, mountain and HQ 4 */
    void *unk_04;
    void *unk_08;
    u32 unk_0c;
    TextRef unk_10;                    /* group 0xc000; likely the name */
    TextRef unk_14;                    /* group 0xc000 */
} TerrainStats;

typedef char TerrainStats_size_check[sizeof(TerrainStats) == 0x18 ? 1 : -1];

extern TerrainStats Ov000_gTerrainStats[31];

#endif
