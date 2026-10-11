#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bba4.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void FS_ChangeDir(const char *dir);
void File_UnpackTo(void *dest, const char *fileName);
void Palette_LoadFromFile(const char *fileName, int fileOffset, int paletteOffset, int size);
void Palette_Load(const void *src, u32 offset, u32 size);
void Palette_Flush(void);
void Data_Unpack(const void *src, void *dst);
void *File_Load(const char *path);
void Mem_Free(void *block);
void Mem_Fill32(u32 value, void *dst, u32 size);
void TileMap_AddToEntries(u16 *entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_ov005_0236aa80[];     /* "/rikiishi" */
extern char data_ov005_0236aa90[];     /* "051": palette */
extern char data_ov005_0236aa98[];     /* "052": tiles */
extern char data_ov005_0236aa9c[];     /* "053": map */
extern char data_ov005_0236aaa0[];     /* "/ohashi/res_totalrank_wall1" */
extern char data_ov005_0236aabc[];     /* "/ohashi/res_totalrank_wall2" */
extern char data_ov005_0236aad8[];     /* "/ohashi/res_totalrank_wall3" */
extern char data_ov005_0236aaf4[];     /* "054": map */
extern u32 data_ov005_02367fac[];      /* map bytes of a text BG by screen size */
extern u32 data_ov005_02367fbc[];      /* map bytes of a rotation/scaling BG by screen size */

/* States 38-40 of the state table (data_ov005_0236a74c), arg 0-2 picks
 * /ohashi/res_totalrank_wall1-3. When the wall's game flag is set
 * (work->wall1Unlocked..wall3Unlocked: flags 61, 62, 34), unpacks its layer 2
 * tiles, map (palette 6) and palette from the packed file on both screens.
 * Otherwise sets work->wallLocked, loads the /rikiishi placeholder (tiles 052,
 * map 053, or 054 for the third wall, palette 051) on the sub screen and
 * clears palettes 6-15 of the palette buffer that was current on entry.
 * Then shows only BG2 and OBJ on the main screen and only BG2 on the sub
 * screen. */
void Ov005State_ShowTotalRankWall(Unk_ov005_0236bba4 *work, int wall)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    u8 *file;
    u8 *palette;
    u16 *map;

    palette = data_027c0280->paletteBuffer;
    switch (wall) {
    case 0:
        file = File_Load(data_ov005_0236aaa0);
        break;
    case 1:
        file = File_Load(data_ov005_0236aabc);
        break;
    case 2:
        file = File_Load(data_ov005_0236aad8);
        break;
    }

    switch (wall) {
    case 0:
        if (work->wall1Unlocked == 0) {
            work->wallLocked = 1;
            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_04);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_04);
            FS_ChangeDir(data_ov005_0236aa80);
            File_UnpackTo(data_027c0280->bgVram
                              + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
                          data_ov005_0236aa98);
            File_UnpackTo(data_027c0280->layer2Map, data_ov005_0236aa9c);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_LoadFromFile(data_ov005_0236aa90, 0, 0xc0, 32);
            Gfx_RequestLayerCopy(2);
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);

            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_00);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_00);
            Mem_Fill32(0, palette + 0xc0, 0x140);
            Palette_Flush();
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        } else {
            saved0280 = data_027c0280;
            saved027c = data_027c027c;
            Screen_SetCurrentBoth(data_027c0288.unk_00);
            Data_Unpack(file + 0x49cc,
                        data_027c0280->bgVram
                            + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
            Data_Unpack(file + 0x9a14, data_027c0280->layer2Map);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_Load(file + 0xa0d0, 0xc0, 0x140);
            Gfx_RequestLayerCopy(2);

            Screen_SetCurrentBoth(data_027c0288.unk_04);
            Data_Unpack(file + 0x49cc,
                        data_027c0280->bgVram
                            + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
            Data_Unpack(file + 0x9a14, data_027c0280->layer2Map);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_Load(file + 0xa0d0, 0xc0, 0x140);
            Gfx_RequestLayerCopy(2);
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        }
        break;
    case 1:
        if (work->wall2Unlocked == 0) {
            work->wallLocked = 1;
            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_04);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_04);
            FS_ChangeDir(data_ov005_0236aa80);
            File_UnpackTo(data_027c0280->bgVram
                              + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
                          data_ov005_0236aa98);
            File_UnpackTo(data_027c0280->layer2Map, data_ov005_0236aa9c);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_LoadFromFile(data_ov005_0236aa90, 0, 0xc0, 32);
            Gfx_RequestLayerCopy(2);
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);

            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_00);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_00);
            Mem_Fill32(0, palette + 0xc0, 0x140);
            Palette_Flush();
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        } else {
            saved0280 = data_027c0280;
            saved027c = data_027c027c;
            Screen_SetCurrentBoth(data_027c0288.unk_00);
            Data_Unpack(file + 0x3a8c,
                        data_027c0280->bgVram
                            + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
            Data_Unpack(file + 0x7864, data_027c0280->layer2Map);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_Load(file + 0x7edc, 0xc0, 0x140);
            Gfx_RequestLayerCopy(2);

            Screen_SetCurrentBoth(data_027c0288.unk_04);
            Data_Unpack(file + 0x3a8c,
                        data_027c0280->bgVram
                            + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
            Data_Unpack(file + 0x7864, data_027c0280->layer2Map);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_Load(file + 0x7edc, 0xc0, 0x140);
            Gfx_RequestLayerCopy(2);
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        }
        break;
    case 2:
        if (work->wall3Unlocked == 0) {
            work->wallLocked = 1;
            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_04);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_04);
            FS_ChangeDir(data_ov005_0236aa80);
            File_UnpackTo(data_027c0280->bgVram
                              + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14),
                          data_ov005_0236aa98);
            File_UnpackTo(data_027c0280->layer2Map, data_ov005_0236aaf4);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_LoadFromFile(data_ov005_0236aa90, 0, 0xc0, 32);
            Gfx_RequestLayerCopy(2);
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);

            saved0280 = data_027c0280;
            Screen_SetCurrentGfx(data_027c0288.unk_00);
            saved027c = data_027c027c;
            Screen_SetCurrent(data_027c0288.unk_00);
            Mem_Fill32(0, palette + 0xc0, 0x140);
            Palette_Flush();
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        } else {
            saved0280 = data_027c0280;
            saved027c = data_027c027c;
            Screen_SetCurrentBoth(data_027c0288.unk_00);
            Data_Unpack(file + 0x3c40,
                        data_027c0280->bgVram
                            + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
            Data_Unpack(file + 0x84b0, data_027c0280->layer2Map);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_Load(file + 0x8b68, 0xc0, 0x140);
            Gfx_RequestLayerCopy(2);

            Screen_SetCurrentBoth(data_027c0288.unk_04);
            Data_Unpack(file + 0x3c40,
                        data_027c0280->bgVram
                            + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
            Data_Unpack(file + 0x84b0, data_027c0280->layer2Map);
            map = data_027c0280->layer2Map;
            TileMap_AddToEntries(map,
                          Screen_IsTextBg(2) ? data_ov005_02367fac[GFX_LAYER(data_027c0280, 2)->screenSize]
                          : Screen_IsAffineBg(2) ? data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize]
                                                 : data_ov005_02367fbc[GFX_LAYER(data_027c0280, 2)->screenSize] * 2,
                          0x6000);
            Palette_Load(file + 0x8b68, 0xc0, 0x140);
            Gfx_RequestLayerCopy(2);
            Screen_SetCurrent(saved027c);
            Screen_SetCurrentGfx(saved0280);
        }
        break;
    }
    Mem_Free(file);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    data_027c0280->dispCnt &= ~0x100;
    data_027c0280->dispCnt &= ~0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt &= ~0x800;
    data_027c0280->dispCnt |= 0x1000;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_027c0280->dispCnt &= ~0x100;
    data_027c0280->dispCnt &= ~0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt &= ~0x800;
    data_027c0280->dispCnt &= ~0x1000;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
