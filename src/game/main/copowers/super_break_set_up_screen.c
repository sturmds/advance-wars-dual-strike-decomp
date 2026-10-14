#include "types.h"
#include "game/menu_state.h"
#include "game/co_data.h"
#include "game/player.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

void func_02021b80(void);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
const char* File_LocalizePath(const char* path);
u8* File_Load(const char* path);
void Mem_Free(void* res);
void Data_Unpack(const void* src, void* dest);
void Mem_Copy32(const void* src, void* dest, u32 size);
void Palette_Load(const void* src, int offset, u32 size);
void Gfx_RequestLayerCopy(int layer);
int Co_GetNation(int co);
void Wallpaper_LoadOnLayer3(u32 index);
void Co_LoadArmyGraphicsAndPalette(int coId, int firstTile, int paletteSlot,
                                   int army);
int Link_IsActive(void);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);
const void* Text_Get(u32 textId);
u8 BreakText_Load(const s8* text, u16 tile, u8 palette, u32 tagFont);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern GfxLayer
    data_0215c050[]; /* layer config while the main screen is current */
extern GfxLayer data_0215c0a0[]; /* layer config otherwise */
extern char data_0215c268[];     /* "/ohashi/res_syogunbreak" */
extern char data_0215c280[];     /* "/ohashi/res_syogunbreak_lang" */
extern char data_0215c2a0[];     /* "/ohashi/res_syogunchange" */

#define GAME (data_027c027c->unk_48[data_027c0284])
#define BANNER_PALETTE_COUNT 3 /* BG palettes 1-3 */
#define PLAYER (&GAME->players[GAME->currentPlayer])
#define LAYER_TILES(layerBg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[layerBg].charBase << 14))

/* First step of the Super CO Power cut-in: sets up the current screen with
 * BG2 blended under the other layers. BG2 comes from res_syogunbreak: the
 * Black Hole backdrop for a Black Hole army in the campaign or a Black Hole
 * CO elsewhere, the allied one otherwise, with the army's palette. Then the
 * BG1 map from the same file with its tiles from the localized
 * res_syogunbreak_lang, the BG0 layer (res_syogunchange),
 * the CO's wallpaper on BG3, the CO picture and the Super CO Power name. */
void SuperBreak_SetUpScreen(Task* task) {
  u8* res;
  int i;

  func_02021b80();
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
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  /* needed to match: the original reads blendEvy and writes it back */
  data_027c0280->blendEvy = (u16)data_027c0280->blendEvy;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendBottomObj = 0;
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
      Data_Unpack(res + 0x418, data_027c0280->layer2Map);
    } else {
      Data_Unpack(res + 0x2d8, data_027c0280->layer2Map);
    }
  } else {
    if (Co_GetNation(PLAYER->front.id & PLAYER_CO_ID_MASK) ==
        NATION_BLACK_HOLE) {
      Data_Unpack(res + 0x418, data_027c0280->layer2Map);
    } else {
      Data_Unpack(res + 0x2d8, data_027c0280->layer2Map);
    }
  }
  Palette_Load(res + 0x8e0 + (PLAYER->armyColour - 1) * 32, 0, 32);
  Gfx_RequestLayerCopy(2);
  Data_Unpack(res + 0x59c, data_027c0280->layer1Map);
  Gfx_RequestLayerCopy(1);
  Mem_Free(res);

  res = File_Load(File_LocalizePath(data_0215c280));
  Data_Unpack(res, LAYER_TILES(data_027c0280->layer1Bg));
  Mem_Free(res);

  res = File_Load(data_0215c2a0);
  Data_Unpack(res, LAYER_TILES(data_027c0280->layer0Bg));
  Data_Unpack(res + 0x2540, data_027c0280->layer0Map);
  Palette_Load(res + 0x2b74, 0x20, 0x60);
  Mem_Copy32(res + 0x2b74, gMenuState.bytes,
             BANNER_PALETTE_COUNT * PALETTE_16_SIZE);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer0Map[i] += 0x1000;
  }
  Gfx_RequestLayerCopy(0);
  Mem_Free(res);

  Wallpaper_LoadOnLayer3((u8)(PLAYER->front.id & PLAYER_CO_ID_MASK));
  Co_LoadArmyGraphicsAndPalette(PLAYER->front.id, 16, 0, GAME->currentPlayer);
  /* needed to match: indexing from &gCoTable[0].blocks[POWER_SUPER] keeps
     * the 0x1a0 offset in the relocation (data_021537ac) */
  task->unk_58 = BreakText_Load(
      Text_Get(
          *(u32*)&(
               (CoRecord*)&gCoTable[0]
                   .blocks[POWER_SUPER])[PLAYER->front.id & PLAYER_CO_ID_MASK]
               .name),
      0x1c0, 1, 0);
  if (data_027c027c != data_027c0288.unk_00 && Link_IsActive() == 1) {
    SpriteTask_Create(232, 8, 0, 15);
  }
  /* a halfword the next steps of this task use (SuperBreak_LoadBanner clears it too) */
  *(u16*)&task->unk_4c = 0;
}
