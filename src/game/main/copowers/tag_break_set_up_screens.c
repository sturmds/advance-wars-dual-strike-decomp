#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void Screen_SetCurrentBoth(void *state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void *config);
u8 *File_Load(const char *path);
void Mem_Free(void *res);
void Data_Unpack(const void *src, void *dest);
void Mem_Copy32(const void *src, void *dest, u32 size);
void Mem_Zero32(u32 value, void *dest, u32 size);
void Palette_Load(const void *src, int offset, u32 size);
void Gfx_RequestLayerCopy(int layer);
int Co_GetNation(int co);
int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadObjPicture(int coId, int tile, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
void Co_LoadArmyGraphics(int coId, int tile, int army);
int Link_IsActive(void);
u32 Co_GetTagPowerNameId(int frontCo, int backCo);   /* text id of the tag power of two COs */
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);
const void *Text_Get(u32 textId);
u8 BreakText_Load(const s8 *text, u16 tile, u8 palette, u32 tagFont);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u8 data_0215c3a4[];             /* layer config while the main screen is current */
extern u8 data_0215c3f4[];             /* layer config otherwise */
extern char data_0215c604[];           /* "/ohashi/res_tagbreak_black" */
extern char data_0215c620[];           /* "/ohashi/res_tagbreak_union" */
extern char data_0215c63c[];           /* "/ohashi/res_tagbreak_mix" */
extern char data_0215c658[];           /* "/ohashi/res_tagbreak" */
extern char data_0215c670[];           /* "/ohashi/res_tagbreak_head" */
extern char data_0215c68c[];           /* "/ohashi/res_tagchange" */
extern u8 data_0229fb70[];
extern u8 gTagChangePalette[];

#define GAME (data_027c027c->unk_48[data_027c0284])
#define PLAYER (&GAME->players[GAME->currentPlayer])
/* Player.front for i = 0, Player.back for i = 1. */
#define PLAYER_CO(i) ((&PLAYER->front)[i].id)
#define MAIN_GFX ((Unk_027c0280 *)data_027c0288.unk_00)
#define LAYER_TILES(layerBg) \
    (data_027c0280->bgVram + (data_027c0280->bgCnt[layerBg].charBase << 14))

/* Default layer setup of one screen: BG3 and OBJ on, no blending, no scroll. */
static inline void SetUpLayers(void)
{
    Screen_SetBgMode(0);
    if (data_027c027c == data_027c0288.unk_00) {
        Screen_InitLayers(data_0215c3a4);
    } else {
        Screen_InitLayers(data_0215c3f4);
    }
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt &= ~0x100;
    data_027c0280->dispCnt &= ~0x200;
    data_027c0280->dispCnt &= ~0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->dispCnt &= ~0x2000;
    data_027c0280->dispCnt &= ~0x4000;
    data_027c0280->dispCnt &= ~0x8000;
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
}

/* Loads the BG3 backdrop from one of the res_tagbreak_* files into the main
 * screen and copies it to the sub screen. */
#define LOAD_BACKDROP(path, mapOffset, paletteOffset)                         \
    do {                                                                      \
        res = File_Load(path);                                                \
        savedGfx = data_027c0280;                                             \
        Screen_SetCurrentGfx(data_027c0288.unk_00);                           \
        Data_Unpack(res, LAYER_TILES(data_027c0280->layer3Bg));               \
        Data_Unpack(res + mapOffset, data_027c0280->layer3Map);               \
        Palette_Load(res + paletteOffset, 0x20, 0x40);                        \
        for (i = 0; i < 0x800; i++) {                                         \
            data_027c0280->layer3Map[i] += 0x1000;                            \
        }                                                                     \
        Gfx_RequestLayerCopy(3);                                              \
        Screen_SetCurrentGfx(savedGfx);                                       \
        savedGfx = data_027c0280;                                             \
        Screen_SetCurrentGfx(data_027c0288.unk_04);                           \
        Mem_Copy32(MAIN_GFX->unk_120, LAYER_TILES(data_027c0280->layer3Bg), 0x6000);\
        Mem_Copy32(MAIN_GFX->layer3Map, data_027c0280->layer3Map, 0x1000);    \
        Palette_Load(res + paletteOffset, 0x20, 0x40);                        \
        Gfx_RequestLayerCopy(3);                                              \
        Screen_SetCurrentGfx(savedGfx);                                       \
        Mem_Free(res);                                                        \
    } while (0)

/* First step of the tag power cut-in: sets up both screens with the same
 * picture. The BG3 backdrop depends on the armies: res_tagbreak_black when
 * both COs are Black Hole, _union when neither is, _mix otherwise; in the
 * campaign the player's army colour decides between black and union. Then
 * the frame (BG1, BG2), both CO pictures, extra head tiles for Grit and
 * Kindle, the res_tagchange layer (BG0) and the tag power name. */
void TagBreak_SetUpScreens(Task *task)
{
    Unk_027c0280 *savedGfx;
    Unk_02080440 *savedScreen;
    Unk_027c0280 *savedGfx2;           /* needed to match: a second variable, kept on the stack */
    u8 *res;
    int tile;
    int i;

    savedGfx = data_027c0280;
    savedScreen = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    SetUpLayers();
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    SetUpLayers();
    Screen_SetCurrent(savedScreen);
    Screen_SetCurrentGfx(savedGfx);

    /* needed to match: the original keeps one copy of the black and union
     * loads, so the campaign test jumps into them */
    if (GAME->gameMode == GAME_MODE_CAMPAIGN) {
        if (PLAYER->armyColour == ARMY_BLACK_HOLE) {
            goto blackHole;
        }
        goto allied;
    } else if (Co_GetNation(PLAYER->front.id & PLAYER_CO_ID_MASK) != NATION_BLACK_HOLE
               || Co_GetNation(PLAYER->back.id & PLAYER_CO_ID_MASK) != NATION_BLACK_HOLE) {
        goto notBlackHole;
    }
blackHole:
    LOAD_BACKDROP(data_0215c604, 0x245c, 0x2a00);
    goto loaded;
notBlackHole:
    if (Co_GetNation(PLAYER->front.id & PLAYER_CO_ID_MASK) != NATION_BLACK_HOLE
        && Co_GetNation(PLAYER->back.id & PLAYER_CO_ID_MASK) != NATION_BLACK_HOLE) {
allied:
        LOAD_BACKDROP(data_0215c620, 0x2b4c, 0x3214);
    } else {
        LOAD_BACKDROP(data_0215c63c, 0x2b48, 0x31d4);
    }
loaded:
    res = File_Load(data_0215c658);
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Data_Unpack(res + 0x380, LAYER_TILES(data_027c0280->layer2Bg));
    Data_Unpack(res + 0x2628, data_027c0280->layer2Map);
    Palette_Load(res + 0x2c60, 0, 0x20);
    Gfx_RequestLayerCopy(2);
    Data_Unpack(res + 0x2b10, LAYER_TILES(data_027c0280->layer1Bg));
    Data_Unpack(res + 0x2b54, data_027c0280->layer1Map);
    Gfx_RequestLayerCopy(1);
    Data_Unpack(res, data_027c0280->objVram + 0xb600);
    Palette_Load(res + 0x2e0, 0x260, 0x80);
    Mem_Copy32(res + 0x360, data_0229fb70, 0x20);
    Screen_SetCurrentGfx(savedGfx);

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Mem_Copy32(MAIN_GFX->unk_10c, LAYER_TILES(data_027c0280->layer2Bg), 0x4000);
    Mem_Copy32(MAIN_GFX->layer2Map, data_027c0280->layer2Map, 0x800);
    Palette_Load(res + 0x2c60, 0, 0x20);
    Gfx_RequestLayerCopy(2);
    Mem_Copy32(MAIN_GFX->unk_f8, LAYER_TILES(data_027c0280->layer1Bg), 0x100);
    Mem_Copy32(MAIN_GFX->layer1Map, data_027c0280->layer1Map, 0x800);
    Gfx_RequestLayerCopy(1);
    Screen_SetCurrentGfx(savedGfx);
    Mem_Free(res);

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Co_LoadArmyGraphics(PLAYER->front.id, 0x10, GAME->currentPlayer);
    Co_LoadObjPicture(PLAYER->front.id, 0x1c0, GAME->currentPlayer);
    Co_LoadArmyPalette(PLAYER->front.id, 16, GAME->currentPlayer);
    Co_LoadArmyGraphics(PLAYER->back.id, 0x2e0, GAME->currentPlayer);
    Co_LoadObjPicture(PLAYER->back.id, 0x490, GAME->currentPlayer);
    Co_LoadArmyPalette(PLAYER->back.id, 17, GAME->currentPlayer);
    Screen_SetCurrentGfx(savedGfx);

    savedGfx2 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Mem_Copy32(MAIN_GFX->objVram + 0x200, data_027c0280->objVram + 0x200, 0xb400);
    Co_LoadArmyPalette(PLAYER->front.id, 16, GAME->currentPlayer);
    Co_LoadArmyPalette(PLAYER->back.id, 17, GAME->currentPlayer);
    res = File_Load(data_0215c670);
    for (i = 0, tile = 0x5b0; i <= 1; i++, tile += 24) {
        if ((PLAYER_CO(i) & PLAYER_CO_ID_MASK) == CO_GRIT && Co_GetArmyPictureVariant(PLAYER_CO(i), GAME->currentPlayer) == 0) {
            Data_Unpack(res, data_027c0280->objVram + ((u32)tile << 5));
        } else if ((PLAYER_CO(i) & PLAYER_CO_ID_MASK) == CO_KINDLE && Co_GetArmyPictureVariant(PLAYER_CO(i), GAME->currentPlayer) == 0) {
            Data_Unpack(res + 0x90, data_027c0280->objVram + ((u32)tile << 5));
        } else if ((PLAYER_CO(i) & PLAYER_CO_ID_MASK) == CO_KINDLE && Co_GetArmyPictureVariant(PLAYER_CO(i), GAME->currentPlayer) == 1) {
            Data_Unpack(res + 0x1a4, data_027c0280->objVram + ((u32)tile << 5));
        } else {
            Mem_Zero32(0, data_027c0280->objVram + ((u32)tile << 5), 0x300);
        }
    }
    Mem_Free(res);
    Screen_SetCurrentGfx(savedGfx2);

    res = File_Load(data_0215c68c);
    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Data_Unpack(res, LAYER_TILES(data_027c0280->layer0Bg));
    Data_Unpack(res + 0x28f0, data_027c0280->layer0Map);
    Palette_Load(res + 0x2cd0, 0x60, 0x60);
    Mem_Copy32(res + 0x2cd0, gTagChangePalette, 0x60);
    for (i = 0; i < 0x300; i++) {
        data_027c0280->layer0Map[i] += 0x3000;
    }
    Gfx_RequestLayerCopy(0);
    Screen_SetCurrentGfx(savedGfx);

    savedGfx = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Mem_Copy32(MAIN_GFX->unk_e4, LAYER_TILES(data_027c0280->layer0Bg), 0x6000);
    Data_Unpack(res + 0x24dc, data_027c0280->layer0Map);
    Palette_Load(res + 0x2cd0, 0x60, 0x60);
    for (i = 0; i < 0x300; i++) {
        data_027c0280->layer0Map[i] += 0x3000;
    }
    Gfx_RequestLayerCopy(0);
    Screen_SetCurrentGfx(savedGfx);
    Mem_Free(res);

    if (Link_IsActive() == 1) {
        SpriteTask_Create(232, 8, 0, 15);
    }
    task->unk_58 = BreakText_Load(Text_Get(Co_GetTagPowerNameId(PLAYER->front.id & PLAYER_CO_ID_MASK,
                                                         PLAYER->back.id & PLAYER_CO_ID_MASK)),
                                  0x61a, 2, 1);

    /* BG1 to BG3 start 192 pixels down on the main screen. */
#define SET_SCROLL_Y(state, layerBg, y)               \
    savedGfx = data_027c0280;                       \
    Screen_SetCurrentGfx(state);                    \
    savedScreen = data_027c027c;                    \
    Screen_SetCurrent(state);                       \
    data_027c0280->bgScrollY[data_027c0280->layerBg] = (y); \
    Screen_SetCurrent(savedScreen);                 \
    Screen_SetCurrentGfx(savedGfx)

    SET_SCROLL_Y(data_027c0288.unk_00, layer1Bg, 192);
    SET_SCROLL_Y(data_027c0288.unk_04, layer1Bg, 0);
    SET_SCROLL_Y(data_027c0288.unk_00, layer2Bg, 192);
    SET_SCROLL_Y(data_027c0288.unk_04, layer2Bg, 0);
    SET_SCROLL_Y(data_027c0288.unk_00, layer3Bg, 192);
    SET_SCROLL_Y(data_027c0288.unk_04, layer3Bg, 0);
}
