#ifndef GAME_MAP_TILESET_FILES_H
#define GAME_MAP_TILESET_FILES_H

#include "types.h"

/* One of the 28-byte records in data_02167e30, per tileset (the tileset
 * byte & 0x7f). Names are short file names in /bmap. */
typedef struct MapTilesetFiles {
  const char*
      palette; /* the map texture palette; MapGfx_LoadGreyscale loads it */
  const char*
      tiles; /* unpacked into layer 3's tile data by MapGfx_LoadTilesetAndWeather */
  const u8*
      bgPalettes; /* three BG palette rows; MapGfx_LoadTilesetPalettes loads them */
  const char*
      objPaletteFile; /* one OBJ palette row; MapGfx_LoadTilesetObjPalette loads it outside the Design Room */
  const char*
      texImage2; /* texture image file; MapGfx_LoadTextures reads it from the record before the tileset's */
  const char* unk_14;    /* a file name; no reader found yet */
  const char* texImage1; /* texture image file, read the same way */
} MapTilesetFiles;

typedef char
    MapTilesetFiles_size_check[sizeof(MapTilesetFiles) == 0x1c ? 1 : -1];

extern MapTilesetFiles data_02167e30[];

#endif
