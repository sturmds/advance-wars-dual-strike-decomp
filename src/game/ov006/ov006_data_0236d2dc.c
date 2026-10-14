#include "types.h"

/* one tile byte (terrain and owner) per tile, by row */
u8 Ov006_gPresetMapTiles[600] = {
#include "data/ov006/Ov006_gPresetMapTiles.inc.c"
};
