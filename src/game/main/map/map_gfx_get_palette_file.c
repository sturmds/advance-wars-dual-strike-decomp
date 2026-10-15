#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_tileset_files.h"

extern Unk_02080440_48 data_02183b08;
extern const char data_0216a1d8[];

/* Palette file of a tileset. Map 0xf8 on the first screen's state uses its
 * own. */
const char* MapGfx_GetPaletteFile(int weather, int tileset) {
  if (MAP_STATE == &data_02183b08 && MAP_STATE->unk_1c4->mapId == 0xf8) {
    return data_0216a1d8;
  }
  return data_02167e30[tileset & 0x7f].palette;
}
