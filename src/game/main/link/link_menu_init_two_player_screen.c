#include "types.h"
#include "game/task.h"
#include "game/design_map.h"
#include "game/menu_state.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_0229fb30.h"

void DesignMap_LoadFromSlot(int slot, DesignMapRecord *dest);
void Gfx_InitBothScreens(void);
void Wallpaper_LoadBoth(void);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void Gfx_LoadBmap09f(void *target, int a, int b);
void Font_LoadDigits(int a);
void LargeFont_Load(void);
void *File_Load(const char *path);
const char *File_LocalizePath(const char *path);
void Data_Unpack(const void *src, void *dst);
void Palette_Load(const void *src, u32 offset, u32 size);
void Palette_BlendWithColor(void *dst, int a, int b, int c);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Mem_Zero32(u32 value, void *dst, u32 size);
void Mem_Free(void *file);
void Gfx_RequestLayerCopy(int layer);
void NumberFont_LoadLow(void);
void ModeFont_Load(int a, int b);
void ModeFont_ShowText(const void *text, int tile, int paletteSlot, int a, int b);
const void *Text_Get(u32 textId);
void func_0202a7ec(const void *text, int tile, int paletteSlot, u8 unk_5c, int unk_58);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);
void Menu_Reset(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void EventQueue_Reset(void);
void PaletteCycle_Start(int a);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern const char gLinkTwoPlayerLoginResPath[];      /* "/commo/res_linklogin" */
extern const char gLinkTwoPlayerCommonLangResPath[]; /* "/commo/res_linkcommon_lang" */
extern const char gLinkTwoPlayerCommonResPath[];     /* "/commo/res_linkcommon" */

/* Script command that sets up the two-player link menu: loads the three
 * saved design maps into MenuState.linkMaps, draws the layer 2 background
 * from the link resources on palette 1, loads OBJ palettes 0-2 (2 blended
 * halfway toward black), shows title 0x7d000000 and text 0x7d000019, and
 * starts at stage 1 with no player's pick made. On the screen
 * data_027c0288.unk_04 it clears the OBJ tiles at 0x8000, where
 * LinkMenu_PreviewMapSlot draws the map preview. */
void LinkMenu_InitTwoPlayerScreen(Task *task)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;
    u8 *file;
    int i;

    for (i = 0; i < 3; i++) {
        DesignMap_LoadFromSlot((u8)i, &LINK_MAPS[i]); /* needed to match: the cast */
    }
    task->waitFrames = 0;
    Gfx_InitBothScreens();
    Wallpaper_LoadBoth();

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    LargeFont_Load();

    file = File_Load(gLinkTwoPlayerLoginResPath);
    Data_Unpack(file + 0x4d4, data_027c0280->layer2Map);
    Palette_Load(file + 0x6cc, 0x20, 0x20);
    Mem_Free(file);
    for (i = 0; i < 0x300; i++) {
        data_027c0280->layer2Map[i] += 0x1000;
    }
    Gfx_RequestLayerCopy(2);

    file = File_Load(File_LocalizePath(gLinkTwoPlayerCommonLangResPath));
    Data_Unpack(file + 0x5e4, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
    Data_Unpack(file, data_027c0280->objVram);
    Mem_Free(file);

    file = File_Load(gLinkTwoPlayerCommonResPath);
    Palette_Load(file, 0x200, 0x20);
    Palette_Load(file, 0x220, 0x20);
    Palette_Load(file, 0x240, 0x20);
    Palette_BlendWithColor(data_027c0280->paletteBuffer + 0x240, 0, 8, 8);
    Mem_Copy32(file + 0x20, &gMenuState, 0x20);
    Mem_Free(file);
    NumberFont_LoadLow();

    ModeFont_Load(0, 0);
    ModeFont_ShowText(Text_Get(0x7d000000), 0, 0, 0, 0);
    func_0202a7ec(Text_Get(0x7d000019), 0, 0, 0, 0);
    SpriteTask_Create(0xe8, 8, 0x3f0, 15);
    Menu_Reset();
    MenuCursor_SetRect(0, -16, 256, -16);
    data_0229fb30.unk_630 = 1;
    data_0229fb30.unk_631 = 0;
    data_0229fb30.unk_632 = 0;
    data_0229fb30.unk_633 = 0;
    for (i = 0; i < 2; i++) {
        ((u8 *)&gMenuState + i)[LINK_PICKED_OFFSET] = 0;
    }
    EventQueue_Reset();

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Mem_Zero32(0, data_027c0280->objVram + 0x8000, 0x4000);
    PaletteCycle_Start(18);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
