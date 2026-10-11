#include "types.h"
#include "sdk/gx.h"
#include "game/main_menu.h"
#include "game/menu_button.h"
#include "game/play_records.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Data_Unpack(const void *src, void *dest);
const char *File_LocalizePath(const char *path);
u8 *File_Load(const char *path);
void File_ReadAt(void *dest, const char *path, u32 offset, u32 size);
void Font_LoadDigits(int a);
void Gfx_LoadBmap09f(void *a, int b, int c);
void HelpBand_Start(int tile, int a);
int MainLoop_GetFrameCount(void);
void *Mem_Alloc(u32 size);
void Mem_Copy32(const void *src, void *dest, u32 size);
void Mem_Free(void *res);
void ModeFont_Load(int tile, int a);
void ModeFont_ShowText(const void *text, int tile, int paletteSlot, int a, int b);
void Palette_Load(const void *src, int offset, u32 size);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Task_JumpToLabel(void *task, int label);
const void *Text_Get(u32 textId);
void Wallpaper_LoadSelected(void);
void MainMenu_BuildPage0(void);
void MainMenu_BuildPage1(void);
void MainMenu_BuildPage2(void);
void MainMenu_BuildPage3(void);
void MainMenu_BuildPage4(void);
void MainMenu_BuildPage5(void);
void MainMenu_BuildPage6(void);
void MainMenu_BuildPage8(void);
void MainMenu_BuildPage9(void);
void MainMenu_BuildPage11(void);
void MainMenu_ShowHelpText(void);
void NumberFont_LoadLow(void);
void Gfx3d_SetOrthoCentered(void);
void Gfx3d_SetupPerspectiveAntialiased(void);
void func_02028978(void);
void TpIcon2_LoadTo(u8 *dest);
void TpIcon_Load(void);
void func_0202d888(void);
void func_0202e8d0(void);
void func_0202e970(void);
void MainMenu_BuildPage10(void);
void MainMenu_BuildPage13(void);
void func_020308c8(void);
void MainMenu_BuildPage7(void);
int func_02102378(void);
s8 func_02102394(void);
s8 func_021023a4(void);
s8 func_021023b4(void);
int func_021023c4(void);
s8 func_021023d4(void);
s8 func_021023e4(void);
void GX_EndLoadTexPltt(void);
void GX_LoadTexPltt(const void *src, u32 offset, u32 size);
void GX_BeginLoadTexPltt(void);
void GX_EndLoadTex(void);
void func_0210cb04(void);
void DC_FlushRange(const void *start, u32 size);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern char data_02159bec[]; /* "/ohashi/res_modemenu_lang" */
extern char data_02159c08[]; /* "/ohashi/res_modemenu" */
extern char data_02159c20[]; /* "/icon/res_icon_cl" */
extern char data_02159c34[]; /* "/ohashi/res_history_medal" */

/* data_0229c71c is declared as MainMenuPage **: it points to a MainMenuWork,
 * whose first field is the page. */
#define WORK ((MainMenuWork *)data_0229c71c)

/* Sets up the main (mode) res: allocates its work block and page, loads
 * res_modemenu_lang, res_modemenu, the res_icon_cl palette and
 * res_history_medal into VRAM and palettes on both screens, then builds a
 * page. On the first visit (MainMenu.unk_11 bit 0 clear) it builds page 0 or
 * 6 from unk_0f_4, puts the cursor on the entry whose kind matches unk_0f_4
 * and sets the default cursor of pages 1-5 and 10 from unlock checks. On a
 * return visit it rebuilds the saved page and jumps the task to label 0.
 * WIP: matches 95%; the res_modemenu pointer gets r6 where the original
 * has r4 (and the loop and save registers shift with it). Tried without
 * effect: separate or shared variables per file, block scopes, loop shapes
 * (while, do, goto, pointer walk, explicit offset), static inline helpers
 * for the loop or the screen loads, void * for the file pointer. */
void func_0202f0d8(MainMenuTask *task)
{
    MainMenuEntry *entry;
    Unk_02080440 *saved027c;
    MainMenuPage *page;
    u8 count;
    int i;
    u8 *res;
    Unk_027c0280 *saved0280;

    Gfx3d_SetupPerspectiveAntialiased();
    *(MainMenuWork **)&data_0229c71c = Mem_Alloc(sizeof(MainMenuWork));
    WORK->page = Mem_Alloc(14); /* 2 header bytes and 6 entries; sizeof(MainMenuPage) counts 7 */
    func_02028978();

    res = File_Load(File_LocalizePath(data_02159bec));
    func_0210cb04();
    Data_Unpack(res, (void *)0x06800000);
    GX_EndLoadTex();
    DC_FlushRange(res + 0x5c78, 32);
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(res + 0x5c78, 0x240, 32);
    GX_EndLoadTexPltt();
    Mem_Free(res);

    res = File_Load(data_02159c08);
    DC_FlushRange(res + 0x1dc, 0x120);
    DC_FlushRange(res + 0x2fc, 0x120);
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(res + 0x1dc, 0, 0x120);
    GX_LoadTexPltt(res + 0x2fc, 0x120, 0x120);
    GX_EndLoadTexPltt();
    for (i = 0; i < 9; i++) {
        WORK->unk_08[i] = *(u16 *)(res + 0x1e4 + i * 32);
    }
    Mem_Copy32(res + 0x41c, WORK->unk_1c, 32);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    Data_Unpack(res, data_027c0280->objVram + 0x140);
    Data_Unpack(res + 0x184, data_027c0280->objVram);
    Palette_Load(res + 0x43c, 0x200, 32);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_04);
    Data_Unpack(res + 0x184, data_027c0280->objVram);
    Palette_Load(res + 0x43c, 0x200, 32);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(res);

    res = Mem_Alloc(32);
    File_ReadAt(res, data_02159c20, 32, 32);
    DC_FlushRange(res, 32);
    GX_BeginLoadTexPltt();
    GX_LoadTexPltt(res, 0x260, 32);
    GX_EndLoadTexPltt();
    Mem_Free(res);

    res = File_Load(data_02159c34);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Data_Unpack(res, data_027c0280->objVram + 0x140);
    Data_Unpack(res + 0x314, data_027c0280->objVram + 0x1c0);
    Palette_Load(res + 0x2b4, 0x260, 32);
    Palette_Load(res + 0x454, 0x280, 32);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(res);

    data_021599d0->slide = 0;
    data_021599d0->unk_10_3 = 0;
    NumberFont_LoadLow();
    TpIcon_Load();
    TpIcon2_LoadTo(data_027c0280->objVram + 0x940);
    Menu_Reset();
    Wallpaper_LoadSelected();
    ModeFont_Load(78, 1);

    if (data_021599d0->unk_11 & 1) {
        ModeFont_ShowText(Text_Get(0x7d00000a), 78, 1, 0, 0);
        switch (data_021599d0->page) {
        case 0:
            MainMenu_BuildPage0();
            break;
        case 6:
            MainMenu_BuildPage6();
            break;
        case 1:
            MainMenu_BuildPage1();
            break;
        case 2:
            MainMenu_BuildPage2();
            break;
        case 3:
            MainMenu_BuildPage3();
            break;
        case 4:
            MainMenu_BuildPage4();
            break;
        case 9:
            MainMenu_BuildPage9();
            break;
        case 5:
            MainMenu_BuildPage5();
            break;
        case 7:
            MainMenu_BuildPage7();
            break;
        case 8:
            MainMenu_BuildPage8();
            break;
        case 10:
            MainMenu_BuildPage10();
            break;
        case 11:
            MainMenu_BuildPage11();
            break;
        case 12:
            func_020308c8();
            break;
        case 13:
            MainMenu_BuildPage13();
            break;
        }
        Task_JumpToLabel(task, 0);
        Gfx3d_SetOrthoCentered();
    } else {
        switch (data_021599d0->unk_0f_4) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 10:
            MainMenu_BuildPage0();
            break;
        default:
            MainMenu_BuildPage6();
            break;
        }

        i = 0;
        data_021599d0->cursors[data_021599d0->page] = i;
        page = *data_0229c71c;
        entry = page->entries;
        count = page->entryCount;
        for (; i < count; i++, entry++) {
            if (entry->hidden) {
                continue;
            }
            switch (data_021599d0->unk_0f_4) {
            case 0:
                if (entry->kind == 0) goto found;
                break;
            case 1:
                if (entry->kind == 2) goto found;
                break;
            case 2:
                if (entry->kind == 3) goto found;
                break;
            case 3:
                if (entry->kind == 1) goto found;
                break;
            case 10:
                if (entry->kind == 4) goto found;
                break;
            case 4:
                if (entry->kind == 9) goto found;
                break;
            case 5:
                if (entry->kind == 8) goto found;
                break;
            case 6:
                if (entry->kind == 5) goto found;
                break;
            case 7:
                if (entry->kind == 7) goto found;
                break;
            case 8:
                if (entry->kind == 10) goto found;
                break;
            case 9:
                if (entry->kind == 6) goto found;
                break;
            }
        }
        goto set_others;
    found:
        data_021599d0->cursors[data_021599d0->page] = i;
    set_others:

        for (i = 0; i < 14; i++) {
            switch (i) {
            case 1:
                if (func_021023e4()) {
                    data_021599d0->cursors[i] = 1;
                } else {
                    data_021599d0->cursors[i] = 0;
                }
                break;
            case 2:
                if (func_021023a4()) {
                    data_021599d0->cursors[i] = 1;
                } else {
                    data_021599d0->cursors[i] = 0;
                }
                break;
            case 3:
                if (func_021023b4()) {
                    data_021599d0->cursors[i] = 1;
                } else {
                    data_021599d0->cursors[i] = 0;
                }
                break;
            case 4:
                if (func_02102394()) {
                    data_021599d0->cursors[i] = 1;
                } else {
                    data_021599d0->cursors[i] = 0;
                }
                break;
            case 5:
                if (func_02102378()) {
                    data_021599d0->cursors[i] = 1;
                } else {
                    data_021599d0->cursors[i] = 0;
                }
                break;
            case 10:
                if (gPlayRecords.unk_11 == 0 && func_021023e4()) {
                    data_021599d0->cursors[i] = 0;
                } else if (gPlayRecords.unk_11 == 1 && func_021023d4()) {
                    data_021599d0->cursors[i] = 1;
                } else if (gPlayRecords.unk_11 == 2 && func_021023c4()) {
                    data_021599d0->cursors[i] = 2;
                } else if (func_021023e4()) {
                    data_021599d0->cursors[i] = 0;
                } else if (func_021023d4()) {
                    data_021599d0->cursors[i] = 1;
                } else {
                    data_021599d0->cursors[i] = 2;
                }
                break;
            }
        }
    }

    func_0202e8d0();
    MenuCursor_SetRect(0, -16, 256, -16);
    MenuButton_Select(data_021599d0->cursors[data_021599d0->page]);
    func_0202e970();
    func_0202d888();
    data_021599d0->unk_11 |= 1;
    HelpBand_Start(0x3f8, 1);
    MainMenu_ShowHelpText();
    task->unk_4c = 0;
    task->unk_52 = MainLoop_GetFrameCount() & 7;
    *REG_SWAP_BUFFERS = 1;
}
