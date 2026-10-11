#include "types.h"
#include "game/campaign_map.h"
#include "game/menu_state.h"
#include "game/task.h"
#include "game/unlocks.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Gfx_InitBothScreens(void);
void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetLayerScreenSize(int layer, int screenSize);
void Gfx_ClearLayerMap(int layer);
u8 *File_Load(const char *path);
const char *File_LocalizePath(const char *path);
void File_UnpackTo(void *dest, const char *fileName);
void File_ReadAt(void *dest, const char *path, u32 offset, u32 size);
void Mem_Free(void *file);
void Data_Unpack(const void *src, void *dst);
void Palette_Load(const void *src, int offset, u32 size);
void Palette_LoadObjRow(int row);
void Gfx_LoadBmap09f(void *target, int a, int b);
void Font_LoadDigits(int a);
void Gfx_RequestLayerCopy(int layer);
int Campaign_IsHardMode(void);
int Score_ToRank(int score);
int CampaignRecords_GetAverageScore(void);
CampaignTopCo *Campaign_GetUnlockedCos(void);
void Icon_LoadTo(int iconId, u8 *vram, int tile);
void IconBank_LoadPalette(int bank, int paletteSlot);
void Co_LoadSmallPicture(int coId, void *dest, int paletteSlot);
void Co_UnpackPicture(int coId, int firstTile);
s8 Text_PrintId(s16 x, s16 y, u16 *map, u32 textId, u16 unk_30, u16 unk_32);
void Text_PrintImmediate(s16 x, s16 y, u16 *map, u32 textId, u16 unk_30, int arg);
void Number_Draw(s16 x, s16 y, u16 *map, u32 value, u16 entryBits, u16 digitSet);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;
extern MenuState gMenuState;
extern const char gTotalRankPath[]; /* "/ohashi/res_totalrank" */
extern const char gTotalRankLangPath[]; /* "/ohashi/res_totalrank_lang" */
extern const char gTotalRankWall3Path[]; /* "/ohashi/res_totalrank_wall3" */
extern const char gTotalRankWall1Path[]; /* "/ohashi/res_totalrank_wall1" */
extern const char gTotalRankWall2Path[]; /* "/ohashi/res_totalrank_wall2" */
extern const char gTotalRankObjPath[]; /* "/ohashi/res_result_obj" */
extern const char gTotalRankTpIconLangPath[]; /* "/ohashi/res_tpicon_lang" */
extern const char gTotalRankIconBlinkPath[]; /* "/ohashi/res_icon_blink" */

#define MAIN_MAP   (data_027c0288.unk_00)
#define SECOND_MAP (data_027c0288.unk_04)

/* SWITCH_TO opens a block that makes `state` the current screen and keeps
 * the previous one; SWITCH_BACK restores it and closes the block. */
#define SWITCH_TO(state)                              \
    {                                                 \
        Unk_027c0280 *saved0280 = data_027c0280;      \
        Unk_02080440 *saved027c;                      \
        Screen_SetCurrentGfx(state);                  \
        saved027c = data_027c027c;                    \
        Screen_SetCurrent(state)

#define SWITCH_BACK()                                 \
        Screen_SetCurrent(saved027c);                 \
        Screen_SetCurrentGfx(saved0280);              \
    }

/* Character data of the BG named by the `layerBg` field of the current screen. */
#define LAYER_CHARS(layerBg) \
    (data_027c0280->bgVram + (data_027c0280->bgCnt[data_027c0280->layerBg].charBase << 14))

/* Frame counter of SubScreenLayer2_SlideInUpdate, in Task.unk_4c. */
#define SLIDE_FRAME(task) (*(s16 *)&(task)->unk_4c)

/* Tile map entries cover 0x300 tiles (32 x 24). */
#define MAP_ENTRIES 0x300

/* Tile map entry bits: palette in bits 12-15. */
#define MAP_PALETTE(n) ((n) << 12)

#define TITLE_TEXT        0x41000002
#define SCORE_SUFFIX_TEXT 0x41000001   /* printed right after each top CO's score */

/* Ranks Score_ToRank returns: 5 from 280 points, 4 from 250, 3 from 200, else
 * 2. The letters are a guess from the game's S, A, B, C ranks. */
#define RANK_S 5
#define RANK_A 4
#define RANK_B 3

#define FLAG_WALL1_UNLOCKED 61
#define FLAG_WALL2_UNLOCKED 62

/* First step of the total rank screen at the end of a campaign. Loads the
 * rank frame and title (res_totalrank, res_totalrank_lang) onto the second
 * screen, and a wallpaper on both: wall3 in hard mode, else wall1 when game
 * flag 60 is set, else wall2. Showing wall1 or wall2 sets its unlock flag.
 * The rank picture follows the average campaign score (CampaignRecords_GetAverageScore). The
 * three top COs of gCampaignUnlockedCos get their picture and score. */
void TotalRankScreen_Init(Task *task)
{
    CampaignTopCo *top;
    u8 *file;                          /* needed to match: res_totalrank, the wallpaper and res_result_obj share it */
    u8 *langFile;
    int i;

    top = Campaign_GetUnlockedCos();
    Gfx_InitBothScreens();
    SWITCH_TO(SECOND_MAP);
    Screen_SetLayerScreenSize(0, 1);
    Screen_SetLayerScreenSize(1, 1);
    Gfx_ClearLayerMap(0);
    Gfx_ClearLayerMap(1);
    Gfx_LoadBmap09f(data_027c0280->unk_f8, 0, 0);
    Font_LoadDigits(0);
    SWITCH_BACK();

    file = File_Load(gTotalRankPath);
    langFile = File_Load(File_LocalizePath(gTotalRankLangPath));
    SWITCH_TO(SECOND_MAP);
    Data_Unpack(file, LAYER_CHARS(layer1Bg) + 0x6000);
    Data_Unpack(file + 0x1d8, data_027c0280->layer1Map);
    Palette_Load(file + 0x2b0, 0x40, 32);
    for (i = 0; i < MAP_ENTRIES; i++) {
        /* the frame's tiles sit 0x300 tiles in (0x6000 bytes) */
        data_027c0280->layer1Map[i] += MAP_PALETTE(2) + 0x300;
    }
    Gfx_RequestLayerCopy(1);
    Data_Unpack(langFile, LAYER_CHARS(layer2Bg));
    Data_Unpack(langFile + 0x10d0, data_027c0280->layer2Map);
    Palette_Load(langFile + 0x13c0, 0x60, 32);
    for (i = 0; i < MAP_ENTRIES; i++) {
        data_027c0280->layer2Map[i] += MAP_PALETTE(3);
    }
    Gfx_RequestLayerCopy(2);
    Data_Unpack(file + 0x2d0, data_027c0280->objVram + 0xc00);
    Palette_Load(file + 0x414, 0x2a0, 32);
    SWITCH_BACK();
    Mem_Free(langFile);
    Mem_Free(file);

    if (Campaign_IsHardMode() == TRUE) {
        file = File_Load(gTotalRankWall3Path);
    } else if (GameFlag_IsSet(60)) {
        file = File_Load(gTotalRankWall1Path);
        GameFlag_Set(FLAG_WALL1_UNLOCKED, 1);
    } else {
        file = File_Load(gTotalRankWall2Path);
        GameFlag_Set(FLAG_WALL2_UNLOCKED, 1);
    }
    SWITCH_TO(MAIN_MAP);
    if (Campaign_IsHardMode() == TRUE) {
        Data_Unpack(file + 0x3c40, LAYER_CHARS(layer3Bg));
        Data_Unpack(file + 0x84b0, data_027c0280->layer3Map);
        Palette_Load(file + 0x8b68, 0x80, 0x140);
    } else if (GameFlag_IsSet(60)) {
        Data_Unpack(file + 0x49cc, LAYER_CHARS(layer3Bg));
        Data_Unpack(file + 0x9a14, data_027c0280->layer3Map);
        Palette_Load(file + 0xa0d0, 0x80, 0x140);
    } else {
        Data_Unpack(file + 0x3a8c, LAYER_CHARS(layer3Bg));
        Data_Unpack(file + 0x7864, data_027c0280->layer3Map);
        Palette_Load(file + 0x7edc, 0x80, 0x140);
    }
    for (i = 0; i < MAP_ENTRIES; i++) {
        data_027c0280->layer3Map[i] += MAP_PALETTE(4);
    }
    Gfx_RequestLayerCopy(3);
    SWITCH_BACK();
    SWITCH_TO(SECOND_MAP);
    if (Campaign_IsHardMode() == TRUE) {
        Data_Unpack(file, LAYER_CHARS(layer3Bg));
        Data_Unpack(file + 0x3628, data_027c0280->layer3Map);
        Palette_Load(file + 0x8b68, 0x80, 0x140);
    } else if (GameFlag_IsSet(60)) {
        Data_Unpack(file, LAYER_CHARS(layer3Bg));
        Data_Unpack(file + 0x4314, data_027c0280->layer3Map);
        Palette_Load(file + 0xa0d0, 0x80, 0x140);
    } else {
        Data_Unpack(file, LAYER_CHARS(layer3Bg));
        Data_Unpack(file + 0x3400, data_027c0280->layer3Map);
        Palette_Load(file + 0x7edc, 0x80, 0x140);
    }
    for (i = 0; i < MAP_ENTRIES; i++) {
        data_027c0280->layer3Map[i] += MAP_PALETTE(4);
    }
    Gfx_RequestLayerCopy(3);
    SWITCH_BACK();
    Mem_Free(file);

    file = File_Load(gTotalRankObjPath);
    SWITCH_TO(SECOND_MAP);
    switch (Score_ToRank(CampaignRecords_GetAverageScore())) {
    case RANK_S:
        Data_Unpack(file, data_027c0280->objVram + 0xf00);
        Palette_Load(file + 0x152c, 0x2c0, 32);
        break;
    case RANK_A:
        Data_Unpack(file + 0x56c, data_027c0280->objVram + 0xf00);
        Palette_Load(file + 0x154c, 0x2c0, 32);
        break;
    case RANK_B:
        Data_Unpack(file + 0x9e4, data_027c0280->objVram + 0xf00);
        Palette_Load(file + 0x156c, 0x2c0, 32);
        break;
    default:
        Data_Unpack(file + 0xd54, data_027c0280->objVram + 0xf00);
        Palette_Load(file + 0x158c, 0x2c0, 32);
        break;
    }
    Data_Unpack(file + 0xfb4, data_027c0280->objVram + 0x1700);
    Palette_Load(file + 0x14cc, 0x2e0, 32);
    SWITCH_BACK();
    Mem_Free(file);

    SWITCH_TO(SECOND_MAP);
    Text_PrintId(1, 14, data_027c0280->layer0Map, TITLE_TEXT, 0, 0x200);
    Gfx_RequestLayerCopy(0);
    File_UnpackTo(data_027c0280->objVram + 0x5c00, File_LocalizePath(gTotalRankTpIconLangPath));
    for (i = 0; i < 3; i++) {
        Icon_LoadTo(188 + i, data_027c0280->objVram, i * 8);
    }
    IconBank_LoadPalette(4, 16);
    i = 0;
    while (TRUE) {
        if (top->co == 0) {
            break;
        }
        Co_LoadSmallPicture(top->co, data_027c0280->objVram + ((24 + i * 12) << 5), 17 + i);
        Co_UnpackPicture(top->co, 60 + i * 12);
        Number_Draw(17, 16 + i * 2, data_027c0280->layer0Map, top->score, 0, 0);
        Text_PrintImmediate(18, 16 + i * 2, data_027c0280->layer0Map, SCORE_SUFFIX_TEXT, 0, 0);
        i++;
        top++;
    }
    Palette_LoadObjRow(4);
    SWITCH_BACK();

    File_ReadAt(&gMenuState, gTotalRankIconBlinkPath, 0, 32);
    SLIDE_FRAME(task) = 0;
    SWITCH_TO(SECOND_MAP);
    GFX_DISPCNT(data_027c0280)->bg0 = 0;
    GFX_DISPCNT(data_027c0280)->bg1 = 0;
    GFX_DISPCNT(data_027c0280)->bg2 = 0;
    GFX_DISPCNT(data_027c0280)->bg3 = 1;
    GFX_DISPCNT(data_027c0280)->obj = 1;
    SWITCH_BACK();
}
