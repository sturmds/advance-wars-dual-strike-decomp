#include "types.h"
#include "game/Unk_02088d04.h"

void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void Palette_LoadFromFile(const char *fileName, int fileOffset, int paletteOffset, int size);
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend);
BOOL func_01ff8ef8(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
extern Unk_027c0280 *data_027c0280;
extern char data_ov004_022bf0dc[];
extern char data_ov004_022bf128[];
extern char data_ov004_022bf12c[];
extern char data_ov004_022bf100[];
extern char data_ov004_022bf130[];
extern char data_ov004_022bf134[];
extern char data_ov004_022bf138[];
extern u32 data_ov004_022bdca4[];
extern u32 data_ov004_022bdcb4[];
#define LAYER_MAP_BYTES(layer, i)                                               \
    (func_01ff8ef8(i) ? data_ov004_022bdca4[(layer)->screenSize]                \
     : Screen_IsAffineBg(i) ? data_ov004_022bdcb4[(layer)->screenSize]          \
                            : data_ov004_022bdcb4[(layer)->screenSize] * 2)

void func_ov004_022bd978(void)
{
    u16 *map;

    FS_ChangeDir(data_ov004_022bf0dc);
    File_UnpackTo(data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14), data_ov004_022bf128);
    File_UnpackTo(GFX_LAYER(data_027c0280, 1)->map, data_ov004_022bf12c);
    map = GFX_LAYER(data_027c0280, 1)->map;
    TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 1), 1), 0x1000);
    Palette_LoadFromFile(data_ov004_022bf100, 0, 32, 32);
    Gfx_RequestLayerCopy(1);

    File_UnpackTo(data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14), data_ov004_022bf130);
    File_UnpackTo(GFX_LAYER(data_027c0280, 2)->map, data_ov004_022bf134);
    map = GFX_LAYER(data_027c0280, 2)->map;
    TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 2), 2), 0x2000);
    Palette_LoadFromFile(data_ov004_022bf138, 0, 64, 256);
    Gfx_RequestLayerCopy(2);
}
