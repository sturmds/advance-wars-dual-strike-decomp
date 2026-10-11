#include "types.h"
#include "game/force_rank_screen.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Bg_PutSkillSlot(u16 x, u16 y, u16 *map, int a);
int Co_GetRank(u32 co);
void Co_UnpackPicture(int coId, int firstTile);
void Data_Unpack(const void *src, void *dest);
u8 *File_Load(const char *path);
const char *File_LocalizePath(const char *path);
void ForceRankScreen_AddButtons(void);
void ForceRankScreen_DrawSlotRow(int x, int y, int textTile, int skill);
void ForceRankScreen_FillColumns(void);
void ForceRankScreen_PutIcons(void);
void Gfx_LoadBmap09f(void *a, int b, int c);
void IconBank_LoadPalette(int a, int b);
void Icon_LoadTo(int a, u8 *objVram, int b);
void Mem_Copy32(const void *src, void *dest, u32 size);
void Mem_Free(void *res);
void MenuButton_Select(int id);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void Menu_Reset(void);
void ModeFont_Load(int tile, int a);
void ModeFont_ShowTextOnScreen(const char *text, int tile, int paletteSlot, int unk_56, u8 unk_5c, int unk_58);
void Palette_Load(const void *src, int offset, u32 size);
void Palette_LoadObjRow(int row);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentBoth(void *state);
void Screen_SetCurrentGfx(void *state);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);
const void *Text_Get(u32 textId);
void Wallpaper_LoadSelected(void);
void Window_DrawOnLayer1(int x, int y, int width, int height);
void func_01ff9b48(void);
void func_0201e6e8(int x, int y, int width, int height);
void Co_LoadGraphicsAndPalette(int co, int tile, int row);
void func_02028a30(void);
void TpIcon_Load(void);
void func_0202b7e0(int a0);
void func_0202b804(void *dest);
void SkillIcon_Load(int skill, void *dest);
void ForceRankScreen_LoadChosen(void);
void ForceRankScreen_AddSlotButtons(void);
int Link_IsActive(void);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_0215a8cc[]; /* "/ohashi/res_skilledit" */
extern char data_0215a8e4[]; /* "/ohashi/res_skilledit_lang" */

#define BG_CHAR_BASE(bg) (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

/* Sets up both screens of the Force Rank screen for gForceRankScreen->co
 * from the res_skilledit files. unk_7a gets the CO's rank; with rank 0 the
 * cursor starts on the "no skill" column. Top screen: one empty slot per
 * rank (at most 4), the CO picture, and the icons. Bottom screen: four
 * slot frames, the CO, and a row per chosen skill. Then the title text
 * 0x7d00000b, the menu, the selected skill's row on the top screen and the
 * buttons.
 * Needed to match: the two per-screen blocks save the current screens in
 * their own locals. */
void ForceRankScreen_Init(void)
{
    u8 *res;
    u8 *lang;
    Unk_027c0280 *savedGfx;
    Unk_027c0280 *savedScreenGfx;
    Unk_02080440 *savedState;
    Unk_02080440 *savedScreenState;
    int slots;
    int i;

    gForceRankScreen->unk_7a = Co_GetRank(gForceRankScreen->co);
    gForceRankScreen->unk_7b = 0;
    gForceRankScreen->cursorColumn = 1;
    if (gForceRankScreen->unk_7a == 0) {
        gForceRankScreen->cursorColumn = FORCE_RANK_COLUMN_NONE;
    }
    gForceRankScreen->cursorRow = 0;
    gForceRankScreen->row = gForceRankScreen->cursorRow;
    ForceRankScreen_LoadChosen();
    ForceRankScreen_FillColumns();
    func_02028a30();

    savedGfx = data_027c0280;
    savedState = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    GFX_DISPCNT(data_027c0280)->objMap1D = 1;
    data_027c0280->unk_dd = 1;
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    GFX_DISPCNT(data_027c0280)->objMap1D = 1;
    data_027c0280->unk_dd = 1;
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);

    res = File_Load(data_0215a8cc);
    lang = File_Load(File_LocalizePath(data_0215a8e4));

    savedScreenGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedScreenState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Data_Unpack(lang, BG_CHAR_BASE(data_027c0280->layer1Bg) + 0x3000);
    Data_Unpack(res, data_027c0280->layer1Map);
    Palette_Load(res + 0x138, 32, 32);
    for (i = 0; i < 0x300; i++) {
        data_027c0280->layer1Map[i] += 0x1180;
    }
    Window_DrawOnLayer1(3, 0, 26, 4);
    slots = gForceRankScreen->unk_7a;
    if (slots > 4) {
        slots = 4;
    }
    for (i = 0; i < slots; i++) {
        Bg_PutSkillSlot(20 + i * 2, 1, data_027c0280->layer1Map, 0);
    }
    func_0201e6e8(0, 20, 28, 4);
    Bg_PutSkillSlot(1, 21, data_027c0280->layer1Map, 0);
    func_0202b804(BG_CHAR_BASE(data_027c0280->layer0Bg) + 0x4000);
    func_0202b7e0(2);
    SkillIcon_Load(0, BG_CHAR_BASE(data_027c0280->layer0Bg) + 0x3f80);
    func_0202b804(data_027c0280->objVram + 0x8000);
    func_0202b7e0(16);
    Co_UnpackPicture(gForceRankScreen->co, 0);
    Palette_LoadObjRow(1);
    Icon_LoadTo(139, data_027c0280->objVram, 12);
    for (i = 0; i < 10; i++) {
        Icon_LoadTo(140 + i, data_027c0280->objVram, 20 + i * 2);
    }
    Icon_LoadTo(213, data_027c0280->objVram, 40);
    for (i = 0; i < 10; i++) {
        Icon_LoadTo(108 + i, data_027c0280->objVram, 44 + i * 2);
    }
    Icon_LoadTo(118, data_027c0280->objVram, 64);
    for (i = 0; i < 10; i++) {
        Icon_LoadTo(120 + i, data_027c0280->objVram, 66 + i * 2);
    }
    Icon_LoadTo(130, data_027c0280->objVram, 86);
    Icon_LoadTo(194, data_027c0280->objVram, 88);
    Icon_LoadTo(137, data_027c0280->objVram, 90);
    IconBank_LoadPalette(4, 18);
    ForceRankScreen_PutIcons();
    Screen_SetCurrent(savedScreenState);
    Screen_SetCurrentGfx(savedScreenGfx);

    savedScreenGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    savedScreenState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    func_0201e6e8(0, 14, 32, 10);
    for (i = 0; i < 4; i++) {
        Bg_PutSkillSlot(1, 15 + i * 2, data_027c0280->layer1Map, 0);
    }
    Co_LoadGraphicsAndPalette(gForceRankScreen->co, 0, 1);
    func_0202b804(BG_CHAR_BASE(data_027c0280->layer0Bg) + 0x4000);
    func_0202b7e0(2);
    slots = gForceRankScreen->unk_7a;
    if (slots > 4) {
        slots = 4;
    }
    for (i = 0; i < slots; i++) {
        ForceRankScreen_DrawSlotRow(1, 15 + i * 2, 256 + i * 64, gForceRankScreen->chosenSkills[i]);
    }
    Screen_SetCurrent(savedScreenState);
    Screen_SetCurrentGfx(savedScreenGfx);

    Mem_Copy32(res + 0x158, gForceRankScreen->cycleColors, 32);
    Mem_Free(res);
    Mem_Free(lang);
    func_01ff9b48();
    TpIcon_Load();
    ModeFont_Load(0x1b0, 2);
    ModeFont_ShowTextOnScreen(Text_Get(0x7d00000b), 0x1b0, 2, 0x800, 2, 0);
    Menu_Reset();
    MenuCursor_SetRect(0, -16, 256, -16);
    MenuButton_Select(0);
    Wallpaper_LoadSelected();

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    savedState = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    ForceRankScreen_DrawSlotRow(1, 21, 256,
                                gForceRankScreen->columns[gForceRankScreen->cursorColumn].skills[gForceRankScreen->cursorRow]);
    Screen_SetCurrent(savedState);
    Screen_SetCurrentGfx(savedGfx);
    ForceRankScreen_AddSlotButtons();
    ForceRankScreen_AddButtons();
    if (Link_IsActive() == 1) {
        SpriteTask_Create(232, 8, 0x7f0, 15);
    }
}
