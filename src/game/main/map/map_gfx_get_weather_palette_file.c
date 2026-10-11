#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern Unk_02080440_48 data_02183b08;
extern const char *gMapPaletteFiles[][5];
extern char gMapF8PaletteFile[];

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Palette file for a weather and tileset; map 0xf8 on the first screen's state has its own. */
const char *MapGfx_GetWeatherPaletteFile(int weather, int tileset)
{
    if (GAME == &data_02183b08 && GAME->unk_1c4->mapId == 0xf8) {
        return gMapF8PaletteFile;
    }
    return gMapPaletteFiles[weather][tileset & 0x7f];
}
