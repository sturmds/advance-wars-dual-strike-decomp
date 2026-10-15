#include "types.h"
#include "game/Unk_02080440.h"

extern const char gTilesetPaletteFileAlt[];
extern const char gTilesetPaletteFileDefault[];

#define VIEW (data_027c027c->unk_48[data_027c0284]->unk_1c4)

/* Name of a palette file for the map's tileset (bit 7 ignored): "0af" for
 * tilesets 2 and 3, "0ae" for the others. Map_LoadExtraObjTiles loads it. */
const char* Tileset_GetPaletteFileName(void) {
  int tileset = VIEW->tileset & 0x7f;

  switch (tileset) {
    case 2:
    case 3:
      return gTilesetPaletteFileAlt;
    default:
      return gTilesetPaletteFileDefault;
  }
}
