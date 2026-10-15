#include "types.h"
#include "game/menu_state.h"
#include "game/player.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/song.h"
#include "game/screen.h"

u8* File_Load(const char* path);
void Mem_Free(void* res);
void Data_Unpack(const void* src, void* dest);
void Palette_Load(const void* src, int offset, u32 size);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Fill32(u32 value, void* dst, u32 size);
void Gfx_RequestLayerCopy(int layer);
void Co_LoadArmyGraphicsAndPalette(
    int coId, int tile, int paletteSlot,
    int army); /* CO picture and its army palette */
int Link_IsActive(void);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);
const void* Text_Get(u32 textId);
u8 BreakText_LoadChangeFont(const s8* text, u16 tile, u8 palette);
void Sound_PlaySe(u32 soundId);

extern GfxLayer
    data_0215c6c4[]; /* layer config while the main screen is current */
extern GfxLayer data_0215c714[]; /* layer config otherwise */
extern char data_0215c7f4[];     /* "/ohashi/res_syogunchange" */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])
#define PLAYER (&MAP_STATE->players[MAP_STATE->currentPlayer])
#define LAYER_TILES(layerBg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[layerBg].charBase << 14))

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

/* Offsets in res_syogunchange. */
#define LAYER0_MAP 0x2540
#define LAYER0_PALETTES 0x2b74 /* three 16-colour palettes, BG palettes 1-3 */
#define LAYER0_PALETTE_COUNT 3
#define LAYER2_TILES 0x2bd4
#define LAYER2_PALETTE 0x2c08

/* Task step that sets up the current screen for the cut-in shown when a tag
 * team swaps COs: BG1-3 shown, no blending, no scroll. BG0 gets the
 * res_syogunchange picture on BG palettes 1-3 (also saved at the start of
 * gMenuState), BG2 a plain fill in the player's army colour. Then both CO
 * pictures, text 0x7d00001d as sprites (their count goes to task->unk_58) and
 * a sound with the song turned down. */
void CoSwapScreen_SetUp(Task* task) {
  u8* res;
  int i;

  Screen_SetBgMode(0);
  if (data_027c027c == data_027c0288.unk_00) {
    Screen_InitLayers(data_0215c6c4);
  } else {
    Screen_InitLayers(data_0215c714);
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

  res = File_Load(data_0215c7f4);
  Data_Unpack(res, LAYER_TILES(data_027c0280->layer0Bg));
  Data_Unpack(res + LAYER0_MAP, data_027c0280->layer0Map);
  Palette_Load(res + LAYER0_PALETTES, 0x20, 0x60);
  Mem_Copy32(res + LAYER0_PALETTES, &gMenuState,
             LAYER0_PALETTE_COUNT * PALETTE_16_SIZE);
  for (i = 0; i < BG_VISIBLE_ENTRIES; i++) {
    data_027c0280->layer0Map[i] += 0x1000;
  }
  Gfx_RequestLayerCopy(0);
  Data_Unpack(res + LAYER2_TILES, LAYER_TILES(data_027c0280->layer2Bg));
  /* every map entry shows tile number armyColour */
  Mem_Fill32(PLAYER->armyColour | (PLAYER->armyColour << 16),
             data_027c0280->layer2Map, BG_SCREEN_SIZE);
  Palette_Load(res + LAYER2_PALETTE, 0, 0x20);
  Gfx_RequestLayerCopy(2);
  Mem_Free(res);

  Co_LoadArmyGraphicsAndPalette(PLAYER->front.id, 16, 0,
                                MAP_STATE->currentPlayer);
  Co_LoadArmyGraphicsAndPalette(PLAYER->back.id, 0x1c0, 1,
                                MAP_STATE->currentPlayer);
  task->unk_58 = BreakText_LoadChangeFont(Text_Get(0x7d00001d), 0x370, 2);
  if (data_027c027c != data_027c0288.unk_00 && Link_IsActive() == 1) {
    SpriteTask_Create(232, 8, 0, 15);
  }
  FRAME(task) = 0;
  Sound_LowerSongVolume(101);
  Sound_PlaySe(81);
}
