#include "types.h"
#include "game/Unk_022a7c5c.h"

const char *func_020f9254(int variant, int style);  /* guess: path of the map palette file */
void func_020f8d40(int variant, int style);         /* guess: loads the map graphics */
u16 Rgb_ToGrey(u16 red, u16 green, u16 blue);
u16 *File_Load(const char *path);
void Mem_Free(void *block);
void func_0210e138(const void *start, u32 size);
void func_0210c8e8(void);
void func_0210c874(const void *src, u32 offset, u32 size);
void func_0210c81c(void);
void WeatherFx_InitClouds(void);

/* Loads the map graphics like MapGfx_Load, but turns the map palette
 * grey before it goes to texture palette VRAM. The palette file is loaded
 * twice: first all 256 colours, then the first 96 again to the same place. */
void MapGfx_LoadGreyscale(int variant, int style)
{
    Unk_022a7c5c *gfx = data_022a7c5c;
    const char *path = func_020f9254(variant, style);
    u16 *palette;
    int i;

    func_020f8d40(variant, style);

    palette = File_Load(path);
    for (i = 0; i < 256; i++) {
        palette[i] = Rgb_ToGrey(palette[i] & 0x1f, (palette[i] & 0x3e0) >> 5, (palette[i] & 0x7c00) >> 10);
    }
    func_0210e138(palette, 256 * sizeof(u16));
    func_0210c8e8();
    func_0210c874(palette, gfx->texPaletteOffset, 256 * sizeof(u16));
    func_0210c81c();
    Mem_Free(palette);

    palette = File_Load(path);
    for (i = 0; i < 96; i++) {
        palette[i] = Rgb_ToGrey(palette[i] & 0x1f, (palette[i] & 0x3e0) >> 5, (palette[i] & 0x7c00) >> 10);
    }
    func_0210e138(palette, 96 * sizeof(u16));
    func_0210c8e8();
    func_0210c874(palette, gfx->texPaletteOffset, 96 * sizeof(u16));
    func_0210c81c();
    Mem_Free(palette);

    WeatherFx_InitClouds();
}
