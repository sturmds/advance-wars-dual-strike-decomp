#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_02160ac0.h"
#include "game/ov003_unit.h"

extern u8 data_ov003_02369be8[];

void Data_Unpack(const void *src, void *dst);
void Palette_Load(const void *src, u32 offset, u32 size);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Gfx_ClearLayerMap(int layer);
void Bg_PutTileBlock(u16 *map, int x, int y, const void *block, int tileAdd);
void Bg_ClearRect(u16 *map, int x, int y, int width, int height);
void Gfx_RequestLayerCopy(int layer);
void SpriteTask_Create(int a, int b, int c, int d);

/* Loads the graphics of the second screen: BG tiles and palettes, object
 * tiles and palettes, a tile block on layer 1 for each of the first 4 rows
 * that is active (at column unk_3b_4 * 4), and BG0's horizontal scroll of
 * -4. With data_02160ac0->unk_7c set it also starts a sprite task. */
void Ov003_LoadSubScreenGraphics(u8 *file, u8 *localizedFile)
{
    int i;
    Ov003Row *row;

    Data_Unpack(localizedFile + 0x3c8, data_027c0280->bgVram);
    Palette_Load(file + 0xb00, 0, 32);
    Data_Unpack(file + 0x98, data_027c0280->bgVram + 0x4000);
    Palette_Load(file + 0x89c, 96, 160);
    Data_Unpack(localizedFile, data_027c0280->objVram + 0x200);
    Palette_Load(file + 0x93c, 512, 32);
    Mem_Copy32(file + 0x95c, data_ov003_02369be8, 32);
    Data_Unpack(file, data_027c0280->objVram + 0x1780);
    Palette_Load(file + 0x89c, 544, 32);
    Gfx_ClearLayerMap(0);
    Gfx_ClearLayerMap(1);
    Gfx_ClearLayerMap(2);
    Gfx_ClearLayerMap(3);
    for (i = 0; i < 4; i++) {
        row = &data_ov003_02364928->rows[i];
        if (row->active) {
            Bg_PutTileBlock(data_027c0280->layer1Map + row->unk_3b_4 * 4, 0, 0, file + 0x97c, 0);
        }
    }
    if (!data_ov003_02364928->unk_1df_6) {
        Bg_ClearRect(data_027c0280->layer1Map, 0, 22, 32, 2);
    }
    Gfx_RequestLayerCopy(1);
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = -4;
    if (data_02160ac0->unk_7c == 0) {
        return;
    }
    SpriteTask_Create(232, 8, 0, 15);
}
