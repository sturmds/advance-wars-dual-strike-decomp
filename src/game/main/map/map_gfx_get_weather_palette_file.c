#include "types.h"
#include "game/Unk_02080440.h"
#include "game/weather.h"

extern Unk_02080440_48 data_02183b08;
extern char gMapF8PaletteFile[];

/* Palette file for a weather and tileset; map 0xf8 on the first screen's state has its own. */
const char* MapGfx_GetWeatherPaletteFile(int weather, int tileset) {
  if (MAP_STATE == &data_02183b08 && MAP_STATE->unk_1c4->mapId == 0xf8) {
    return gMapF8PaletteFile;
  }
  return gMapPaletteFiles[weather].palettes[tileset & 0x7f];
}
