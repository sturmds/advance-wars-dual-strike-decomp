#include "types.h"
#include "game/menu_state.h"
#include "game/page_strip.h"
#include "game/Unk_022a7b80.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Data_Unpack(const void *src, void *dest);
u8 *File_Load(const char *path);
const char *File_LocalizePath(const char *path);
void Font_LoadDigits(int a);
void Gfx_InitBothScreens(void);
void Gfx_LoadBmap09f(void *a, int b, int c);
void Gfx_RequestLayerCopy(int layer);
s32 HwDiv32(s32 numerator, s32 denominator);
s32 HwMod32(s32 numerator, s32 denominator);
void IconBank_LoadPalette(int bank, int paletteSlot);
void Icon_LoadTo(int iconId, u8 *vram, int tile);
void Layer0_PutNumber(int x, int y, u32 value);
void Layer0_PutNumberThenText4(int x, int y, u32 value);
void Layer0_PutRatio(int x, int y, u32 first, u32 second);
void MapPreview_Draw(void *dst, int mapId);
const void *Map_GetName(u32 mapId);
void Mem_Copy32(const void *src, void *dest, u32 size);
void Mem_Free(void *res);
void Menu_Reset(void);
void ModeFont_Load(int a, int b);
void ModeFont_ShowText(const void *text, int tile, int paletteSlot, int a, int b);
void PaletteCycle_Start(int a);
void Palette_Load(const void *src, int offset, u32 size);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentBoth(void *state);
void Screen_SetCurrentGfx(void *state);
const void *Text_Get(u32 textId);
void Text_PrintImmediate(int x, int y, u16 *map, u32 textId, u16 unk_30, int arg);
void Text_PrintStringImmediate(s16 x, s16 y, u16 *map, const void *text, u16 unk_30, int arg);
void Wallpaper_LoadBoth(void);
void Window_Draw(int x, int y, int width, int height, u16 *map, int palette);
void NumberFont_LoadLow(void);
void func_0201e6e8(int x, int y, int width, int height);
void func_0202a7ec(const void *text, int tile, int paletteSlot, u8 unk_5c, int unk_58);
void SurvivalProgress_LoadEnemyCoPictures(PageStripTask *task);
int Survival_GetMapCount(int mode);
int Survival_GetMapId(int mode, int index);

extern Unk_022a7b80 data_022a7b80;
extern MenuState gMenuState;
extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_0215af58[]; /* "/ohashi/res_survival" */
extern char data_0215af70[]; /* "/ohashi/res_survival_lang" */

/* Init of the task that shows how far a Survival run has got, with the
 * res_survival files. data_022a7b80.mapIndex is the run's current map index:
 * the page strip gets one tab per map played so far, wrapping into pages
 * when that passes the mode's map count (Survival_GetMapCount). Top screen: mode
 * title, the strip, the current map's preview and name. Bottom screen: the
 * mode's value from data_022a7b80.budgetLeft, as in Mode_PrintVariantValue.
 * The fields set here sit where PageStripTask_Create copies PageStripTask fields
 * from its creator (its pointer sits next to this one in data_0215af10),
 * so the task uses that struct. */
void SurvivalProgress_Init(PageStripTask *task)
{
    u8 *res;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    u16 *map;

    task->cursor = data_022a7b80.mapIndex;
    task->unk_66 = data_022a7b80.mapIndex + 1;
    task->count = task->unk_66;
    task->pages = 1;
    if (task->count > Survival_GetMapCount(data_022a7b80.mode)) {
        task->count = Survival_GetMapCount(data_022a7b80.mode);
        task->cursor = HwMod32(task->cursor, task->count);
        task->pages = HwDiv32(task->unk_66 - 1, task->count) + 1;
    }
    task->mapId = Survival_GetMapId(data_022a7b80.mode, task->cursor);

    Gfx_InitBothScreens();
    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    ModeFont_Load(0, 0);
    switch (data_022a7b80.mode) {
    case 0:
    case 3:
        ModeFont_ShowText(Text_Get(0x7d00000c), 0, 0, 0, 0);
        break;
    case 2:
    case 5:
        ModeFont_ShowText(Text_Get(0x7d00000d), 0, 0, 0, 0);
        break;
    default:
        ModeFont_ShowText(Text_Get(0x7d00000e), 0, 0, 0, 0);
        break;
    }
    func_0202a7ec(Text_Get(0x7d00000f), 0, 0, 0, 0);
    Wallpaper_LoadBoth();

    res = File_Load(data_0215af58);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    Data_Unpack(res + 0x2dc, data_027c0280->objVram + 0xb00);
    if (task->pages <= 1) {
        Palette_Load(res + 0xe58, 0x2c0, 32);
    } else {
        Palette_Load(res + 0xe58 + ((task->pages - 2) & 7) * 32, 0x2c0, 32);
        Palette_Load(res + 0xe58 + ((task->pages - 1) & 7) * 32, 0x2e0, 32);
    }
    Icon_LoadTo(203, data_027c0280->objVram, 72);
    IconBank_LoadPalette(4, 24);
    MapPreview_Draw(data_027c0280->objVram + 0x8000, (s16)task->mapId); /* needed to match: read signed here only */
    SurvivalProgress_LoadEnemyCoPictures(task);
    map = data_027c0280->layer0Map;
    Text_PrintStringImmediate(12, 21, map, Map_GetName(task->mapId), 0, 0);
    Window_Draw(11, 20, 10, 4, data_027c0280->layer1Map, 0);
    Gfx_RequestLayerCopy(0);
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrentGfx(saved0280);
    Mem_Copy32(res + 0xf78, gMenuState.colours, 32);
    Mem_Free(res);

    res = File_Load(File_LocalizePath(data_0215af70));
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Data_Unpack(res, data_027c0280->objVram + 0x3340);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(res);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    func_0201e6e8(8, 14, 16, 4);
    if (data_022a7b80.mode == 0 || data_022a7b80.mode == 3) {
        Text_PrintImmediate(9, 15, data_027c0280->layer0Map, 0x79000007, 0, 0);
        Layer0_PutRatio(22, 15, HwDiv32(data_022a7b80.budgetLeft, 3600),
                        HwMod32(HwDiv32(data_022a7b80.budgetLeft, 60), 60));
    } else if (data_022a7b80.mode == 2 || data_022a7b80.mode == 5) {
        Text_PrintImmediate(9, 15, data_027c0280->layer0Map, 0x7900000a, 0, 0);
        Layer0_PutNumber(22, 15, data_022a7b80.budgetLeft);
    } else {
        Text_PrintImmediate(9, 15, data_027c0280->layer0Map, 0x7900000d, 0, 0);
        Layer0_PutNumberThenText4(22, 15, data_022a7b80.budgetLeft);
    }
    Gfx_RequestLayerCopy(0);
    Screen_SetCurrentGfx(saved0280);

    NumberFont_LoadLow();
    PaletteCycle_Start(25);
    Menu_Reset();
}
