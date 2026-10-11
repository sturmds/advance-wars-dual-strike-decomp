#include "types.h"
#include "game/screen_other_map.h"

void Gfx_InitBothScreens(void);
void Gfx_RequestLayerCopy(int layer);
void *File_Load(const char *path);
void Mem_Free(void *file);
void Data_Unpack(const void *src, void *dst);
void Palette_Load(const void *src, u32 offset, u32 size);
void ModeFont_Load(int a, int b);
void ModeFont_ShowText(const void *text, int a, int b, int c, int d);
const void *Text_Get(u32 textId);

extern const char gLoadingRes1Path[]; /* "/ohashi/res_loading1" */
extern const char gLoadingRes2Path[]; /* "/ohashi/res_loading2" */

/* Sets up the loading screen: /ohashi/res_loading1 goes to layer 3 of the
 * lower screen (data_027c0288.unk_04), /ohashi/res_loading2 to layer 3 of the
 * upper one, each with its tiles, map and first 128 palette colours. Then shows mode
 * text 0x7d000008. */
void LoadingScreen_Init(void)
{
    u8 *file;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    Gfx_InitBothScreens();

    file = File_Load(gLoadingRes1Path);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Data_Unpack(file, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
    Data_Unpack(file + 0x4400, data_027c0280->layer3Map);
    Palette_Load(file + 0x49c4, 0, 0x100);
    Gfx_RequestLayerCopy(3);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(file);

    file = File_Load(gLoadingRes2Path);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Data_Unpack(file, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
    Data_Unpack(file + 0x35e4, data_027c0280->layer3Map);
    Palette_Load(file + 0x3b88, 0, 0x100);
    Gfx_RequestLayerCopy(3);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(file);

    ModeFont_Load(0, 0);
    ModeFont_ShowText(Text_Get(0x7d000008), 0, 0, 0, 0);
}
