#include "types.h"
#include "game/Unk_022a7c5c.h"
#include "sdk/os.h"

const char *MapGfx_GetPaletteFile(int variant, int style);  /* guess: path of the map palette file */
void MapGfx_LoadTextures(int variant, int style);         /* guess: loads the map graphics */
u16 Rgb_ToGrey(u16 red, u16 green, u16 blue);
u16 *File_Load(const char *path);
void Mem_Free(void *block);
void GX_BeginLoadTexPltt(void);
void GX_LoadTexPltt(const void *src, u32 offset, u32 size);
void GX_EndLoadTexPltt(void);
void WeatherFx_InitClouds(void);

/* Loads the map graphics like MapGfx_Load, but turns the map palette
 * grey before it goes to texture palette VRAM. The palette file is loaded
 * twice: first all 256 colours, then the first 96 again to the same place. */
void MapGfx_LoadGreyscale(int variant, int style)
{
    Unk_022a7c5c *gfx = data_022a7c5c;
    const char *path = MapGfx_GetPaletteFile(variant, style);
    u16 *palette;
    int i;

    MapGfx_LoadTextures(variant, style);

    palette = File_Load(path);
    for (i = 0; i < 256; i++) {
        palette[i] = Rgb_ToGrey(palette[i] & 0x1f, (palette[i] & 0x3e0) >> 5, (palette[i] & 0x7c00) >> 10);
    }
    DC_FlushRange(palette, 256 * sizeof(u16));
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(palette, gfx->texPaletteOffset, 256 * sizeof(u16));
    GX_EndLoadTexPltt();
    Mem_Free(palette);

    palette = File_Load(path);
    for (i = 0; i < 96; i++) {
        palette[i] = Rgb_ToGrey(palette[i] & 0x1f, (palette[i] & 0x3e0) >> 5, (palette[i] & 0x7c00) >> 10);
    }
    DC_FlushRange(palette, 96 * sizeof(u16));
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(palette, gfx->texPaletteOffset, 96 * sizeof(u16));
    GX_EndLoadTexPltt();
    Mem_Free(palette);

    WeatherFx_InitClouds();
}
