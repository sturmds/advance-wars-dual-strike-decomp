#include "types.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void *config);
u8 *File_Load(const char *path);
void Mem_Free(void *res);
void Data_Unpack(const void *src, void *dest);
void Palette_Load(const void *src, int offset, u32 size);
void Gfx_RequestLayerCopy(int layer);
int Co_GetNation(int co);
void Co_LoadArmyGraphicsAndPalette(int coId, int tile, int paletteSlot, int army);   /* CO picture and its army palette */
int Link_IsActive(void);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);
const void *Text_Get(u32 textId);
u8 BreakText_Load(const s8 *text, u16 tile, u8 palette, u32 tagFont);
void PlayCoPowerTheme(int player);

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u8 data_0215c050[];             /* layer config while the main screen is current */
extern u8 data_0215c0a0[];             /* layer config otherwise */
extern char data_0215c268[];           /* "/ohashi/res_syogunbreak" */

#define GAME (data_027c027c->unk_48[data_027c0284])
#define PLAYER (&GAME->players[GAME->currentPlayer])
#define LAYER_TILES(layerBg) \
    (data_027c0280->bgVram + (data_027c0280->bgCnt[layerBg].charBase << 14))

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16 *)&(task)->unk_4c)

/* Offsets in res_syogunbreak. */
#define MAP_ALLIED      0x2d8
#define MAP_BLACK_HOLE  0x418
#define PALETTES        0x8e0          /* one 16-colour palette per army colour, from 1 */

/* Task step that sets up the current screen for the CO Power cut-in: all
 * layers shown, no blending, no scroll. BG2 gets the res_syogunbreak picture
 * in the player's army colour, with the Black Hole map when the CO is Black
 * Hole (in the campaign: when the army is). Then the CO picture, the power
 * name as sprites (their count goes to task->unk_58), and the CO Power song. */
void CoPowerScreen_SetUp(Task *task)
{
    u8 *res;

    Screen_SetBgMode(0);
    if (data_027c027c == data_027c0288.unk_00) {
        Screen_InitLayers(data_0215c050);
    } else {
        Screen_InitLayers(data_0215c0a0);
    }
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt |= 0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
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
    data_027c0280->unk_dd = 0;
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;

    res = File_Load(data_0215c268);
    Data_Unpack(res, LAYER_TILES(data_027c0280->layer2Bg));
    if (GAME->gameMode == GAME_MODE_CAMPAIGN) {
        if (PLAYER->armyColour == ARMY_BLACK_HOLE) {
            Data_Unpack(res + MAP_BLACK_HOLE, data_027c0280->layer2Map);
        } else {
            Data_Unpack(res + MAP_ALLIED, data_027c0280->layer2Map);
        }
    } else if (Co_GetNation(PLAYER->front.id & PLAYER_CO_ID_MASK) == NATION_BLACK_HOLE) {
        Data_Unpack(res + MAP_BLACK_HOLE, data_027c0280->layer2Map);
    } else {
        Data_Unpack(res + MAP_ALLIED, data_027c0280->layer2Map);
    }
    Palette_Load(res + PALETTES + (PLAYER->armyColour - 1) * 32, 0, 32);
    Gfx_RequestLayerCopy(2);
    Mem_Free(res);

    Co_LoadArmyGraphicsAndPalette(PLAYER->front.id, 16, 0, GAME->currentPlayer);
    task->unk_58 = BreakText_Load(Text_Get(gCoTable[PLAYER->front.id & PLAYER_CO_ID_MASK].blocks[POWER_CO].name.id),
                                  0x1c0, 1, 0);
    if (data_027c027c != data_027c0288.unk_00 && Link_IsActive() == 1) {
        SpriteTask_Create(232, 8, 0, 15);
    }
    FRAME(task) = 0;
    PlayCoPowerTheme(GAME->currentPlayer);
}
