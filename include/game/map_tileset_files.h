#ifndef GAME_MAP_TILESET_FILES_H
#define GAME_MAP_TILESET_FILES_H

#include "types.h"

/* One of the 28-byte records in data_02167e30, per tileset (the tileset
 * byte & 0x7f). Names are short file names in /bmap. */
typedef struct MapTilesetFiles {
    const char *palette;               /* the map texture palette; MapGfx_LoadGreyscale loads it */
    const char *tiles;                 /* unpacked into layer 3's tile data by MapGfx_LoadTilesetAndWeather */
    u8 unk_08[0x1c - 0x08];
} MapTilesetFiles;

extern MapTilesetFiles data_02167e30[];

#endif
