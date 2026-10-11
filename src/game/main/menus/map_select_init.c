#include "types.h"
#include "game/map_select.h"
#include "game/menu_state.h"
#include "game/Unk_02088d04.h"
#include "game/event_queue.h"

void Gfx_InitBothScreens(void);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetLayerCharBase(int a, u32 b);
void Screen_SetLayerScreenSize(int layer, int screenSize);
void Gfx_ClearLayerMap(int layer);
void *File_Load(const char *path);
const char *File_LocalizePath(const char *path);
void File_ReadAt(void *dest, const char *path, u32 offset, u32 size);
void Mem_Free(void *file);
void Mem_Copy32(const void *src, void *dst, u32 size);
void Mem_Zero32(u32 value, void *dst, u32 size);
void Data_Unpack(const void *src, void *dst);
void Palette_Load(const void *src, u32 offset, u32 size);
u8 *Palette_GetBlock(int index, int player);
const void *Terrain_GetPalette(int id, int army);
void Palette_BlendWithColor(void *dst, int a, int b, int c);
void Gfx_LoadBmap09f(void *target, int a, int b);
void Font_LoadDigits(int a);
void Gfx_RequestLayerCopy(int layer);
void Icon_LoadTo(int iconId, u8 *vram, int tile);
void IconBank_LoadPalette(int bank, int paletteSlot);
void Window_Draw(int x, int y, int width, int height, u16 *map, int palette);
int Link_IsActive(void);
void MapSelect_BuildWarRoomLists(MapList *lists);
void MapSelect_BuildLists(MapList *lists, int a);
void MapSelect_DrawMapNames(int list, int page);
void MapSelect_LoadWarRoomRecords(void);
void MapSelect_LoadEnemyCoPictures(void);
void func_01ff9b48(void);
void TpIcon_Load(void);
void TpIcon3_LoadTo(u8 *dest);
void MapSelect_ShowSelectedMap(void);
void PaletteCycle_Start(int a);
void ModeFont_Load(int a, int b);
const void *Text_Get(u32 textId);
void ModeFont_ShowTextOnScreen(const void *text, int tile, int paletteSlot, int unk_56, u8 unk_5c, int unk_58);
void Menu_Reset(void);
void MenuCursor_SetRect(s16 left, s16 top, s16 right, s16 bottom);
void MenuButton_Select(int button);
void MapSelect_AddTouchButtons(void);
void MenuButtons_DisableTouch(void);
void Wallpaper_LoadBoth(void);
void SpriteTask_Create(int a, int b, int c, int d);

extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuStateFrom400 data_0229ef30;
extern const char gMapSelectResPath[];
extern const char gMapSelectLangResPath[];
extern const char gIconBlinkResPath[];
extern const char gMapSelectLinkCommonLangResPath[];
extern const char gMapSelectLinkCommonResPath[];

#define MAIN_MAP   (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

#define SWITCH_TO(state)            \
    saved0280 = data_027c0280;      \
    Screen_SetCurrentGfx(state);    \
    saved027c = data_027c027c;      \
    Screen_SetCurrent(state)

#define SWITCH_BACK()               \
    Screen_SetCurrent(saved027c);   \
    Screen_SetCurrentGfx(saved0280)

/* Mode title shown at the top of the map select screen. */
#define TEXT_MAP_SELECT 0x7d000005

/* Sets up the map select screen: blending on both screens, the background
 * and icons from /ohashi/res_mapselect (a different background in War Room
 * mode and for two-map battles), windows and palettes on the second screen,
 * and the menu colours from /ohashi/res_icon_blink. With Link_IsActive()
 * == 1 it also loads /commo/res_linkcommon and makes a sprite task. Then it
 * builds the map lists, resets saved positions that are past the end of
 * their list, draws the current page's map names and the selected map,
 * and selects the button of the selected map. */
void MapSelect_Init(void)
{
    int i;
    u8 *file;
    u8 *langFile;
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    Gfx_InitBothScreens();

    SWITCH_TO(MAIN_MAP);
    Screen_SetLayerScreenSize(0, 1);
    Gfx_ClearLayerMap(0);
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 10;
    data_027c0280->blendEvb = 6;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 1;
    SWITCH_BACK();

    SWITCH_TO(SECOND_MAP);
    Screen_SetLayerCharBase(2, 0x6200000);
    Screen_SetLayerScreenSize(0, 2);
    Screen_SetLayerScreenSize(1, 1);
    Gfx_ClearLayerMap(0);
    Gfx_ClearLayerMap(1);
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 10;
    data_027c0280->blendEvb = 6;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 1;
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    SWITCH_BACK();

    file = File_Load(gMapSelectResPath);
    langFile = File_Load(File_LocalizePath(gMapSelectLangResPath));
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(MAIN_MAP);
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 2;
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
        Data_Unpack(file + 0x1f8, data_027c0280->layer1Map);
        Data_Unpack(langFile + 0x770, data_027c0280->objVram);
        Palette_Load(file + 0x1d8, 0x200, 0x20);
    } else {
        if (STATE->twoMaps == 0) {
            Data_Unpack(file + 0x20, data_027c0280->layer1Map);
        } else {
            Data_Unpack(file + 0xfc, data_027c0280->layer1Map);
        }
        Data_Unpack(langFile, data_027c0280->objVram);
        Palette_Load(file, 0x200, 0x20);
    }
    Gfx_RequestLayerCopy(1);
    for (i = 0; i < 10; i++) {
        Icon_LoadTo(i + 150, data_027c0280->objVram, 0x180 + i * 2);
    }
    Icon_LoadTo(162, data_027c0280->objVram, 0x194);
    IconBank_LoadPalette(4, 17);
    Icon_LoadTo(200, data_027c0280->objVram, 0x196);
    Icon_LoadTo(201, data_027c0280->objVram, 0x19a);
    for (i = 0; i < 3; i++) {
        Icon_LoadTo(i + 188, data_027c0280->objVram, 0x19e + i * 8);
    }
    for (i = 0; i < 10; i++) {
        Icon_LoadTo(i + 108, data_027c0280->objVram, 0x1b6 + i * 2);
    }
    Icon_LoadTo(85, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
                0x3f0 - ROWS * COLS * 32);
    Icon_LoadTo(86, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
                0x3f4 - ROWS * COLS * 32);
    Icon_LoadTo(87, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
                0x3f8 - ROWS * COLS * 32);
    Icon_LoadTo(88, data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layer0Bg].charBase << 14),
                0x3fc - ROWS * COLS * 32);
    IconBank_LoadPalette(1, 1);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(langFile);
    Mem_Free(file);

    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(SECOND_MAP);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    Window_Draw(11, 20, 10, 4, data_027c0280->layer2Map, 0);
    Gfx_RequestLayerCopy(2);
    Icon_LoadTo(203, data_027c0280->objVram, 0);
    for (i = 0; i < 10; i++) {
        Icon_LoadTo(i + 0x10a, data_027c0280->objVram, 0x68 + i * 2);
    }
    IconBank_LoadPalette(4, 19);
    Window_Draw(-1, 16, 10, 9, data_027c0280->layer0Map, 0);
    Gfx_RequestLayerCopy(0);
    Mem_Copy32(Palette_GetBlock(14, 0), data_027c0280->objVram + 0xf80, 0x100);
    Mem_Zero32(0, data_027c0280->objVram + 0xf80, 0x80);
    Mem_Copy32(Palette_GetBlock(10, 0), data_027c0280->objVram + 0x1080, 0x100);
    Mem_Copy32(Palette_GetBlock(11, 0), data_027c0280->objVram + 0x1180, 0x100);
    Mem_Copy32(Palette_GetBlock(6, 0), data_027c0280->objVram + 0x1280, 0x100);
    Mem_Copy32(Palette_GetBlock(22, 0), data_027c0280->objVram + 0x1380, 0x100);
    Mem_Copy32(Palette_GetBlock(17, 0), data_027c0280->objVram + 0x1480, 0x100);
    Palette_Load(Terrain_GetPalette(6, 0), 0x340, 0x20);
    Palette_Load(Terrain_GetPalette(6, 0), 0x360, 0x20);
    Palette_BlendWithColor(data_027c0280->paletteBuffer + 0x360, 0, 8, 0);
    Screen_SetCurrentGfx(saved0280);

    File_ReadAt(&gMenuState, gIconBlinkResPath, 0, 0x20);
    if (Link_IsActive() == 1) {
        file = File_Load(File_LocalizePath(gMapSelectLinkCommonLangResPath));
        Data_Unpack(file, data_027c0280->objVram + 0xb880);
        Mem_Free(file);
        file = File_Load(gMapSelectLinkCommonResPath);
        Palette_Load(file, 0x260, 0x20);
        Mem_Copy32(file + 0x20, MENU->setup.colours2, 0x20);
        Mem_Free(file);
    }
    if (STATE->gameMode == GAME_MODE_WAR_ROOM) {
        MapSelect_BuildWarRoomLists(MENU->setup.lists);
    } else {
        MapSelect_BuildLists(MENU->setup.lists, 0);
    }
    if (LIST_OF_TAB(TAB()) >= MENU->setup.mapListCount) {
        LIST_OF_TAB(TAB()) = 0;
    }
    for (i = 0; i < MENU->setup.mapListCount; i++) {
        if (TABLES.unk_000[TAB()][MENU->setup.lists[i].unk_64][0] >= MENU->setup.lists[i].mapCount) {
            TABLES.unk_000[TAB()][MENU->setup.lists[i].unk_64][0] = 0;
        }
    }
    MapSelect_DrawMapNames(LIST_OF_TAB(TAB()), PAGE());
    data_0229ef30.prevMapId = SELECTED_MAP_ID();
    MENU->setup.prevList = LIST_OF_TAB(TAB());
    data_0229ef30.unk_4fc = 0;
    MapSelect_LoadWarRoomRecords();
    MapSelect_LoadEnemyCoPictures();
    func_01ff9b48();
    TpIcon_Load();
    TpIcon3_LoadTo(data_027c0280->objVram + 0x4b40);
    MapSelect_ShowSelectedMap();
    PaletteCycle_Start(18);
    SWITCH_TO(SECOND_MAP);
    PaletteCycle_Start(18);
    SWITCH_BACK();

    ModeFont_Load(0x200, 0);
    ModeFont_ShowTextOnScreen(Text_Get(TEXT_MAP_SELECT), 0x200, 0, 0xc00, 4, 0);
    Menu_Reset();
    MenuCursor_SetRect(0, -16, 0x100, -16);
    MenuButton_Select(TABLES.unk_000[TAB()][LIST(TAB()).unk_64][PAGE()]);
    MapSelect_AddTouchButtons();
    MenuButtons_DisableTouch();
    Wallpaper_LoadBoth();
    MENU->setup.slideFrames = 0;
    MENU->setup.subSlideFrames = 0;
    EventQueue_Reset();
    if (Link_IsActive() == 1) {
        SpriteTask_Create(0xe8, 8, 0x58, 15);
    }
}
