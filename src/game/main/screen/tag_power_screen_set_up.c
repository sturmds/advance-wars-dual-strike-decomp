#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/co_data.h"
#include "game/task.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);
void Screen_SetCurrentBoth(void* state);
void Screen_SetBgMode(int mode);
void Screen_InitLayers(const void* config);
void* File_Load(const char* path);
void Data_Unpack(const void* src, void* dst);
void Palette_Load(const void* src, u32 offset, u32 size);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Mem_Free(void* block);
void Gfx_RequestLayerCopy(int layer);
int Co_GetNation(int co);
int Co_GetArmyPictureVariant(int coId, int army);
void Co_LoadObjPicture(int coId, int tile, int army);
void Co_LoadArmyPalette(int coId, int paletteSlot, int army);
void Co_LoadArmyGraphics(int coId, int tile, int army);
int Link_IsActive(void);
void SpriteTask_Create(int x, int y, int tile, int paletteSlot);
const void* Text_Get(u32 textId);
u8 BreakText_LoadChangeFont(const s8* text, u16 tile, u8 palette);
void Sound_LowerSongVolume(int volume);
void Sound_PlaySe(u32 soundId);

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern int data_027c0284;
extern Unk_027c0288 data_027c0288;
extern u8 data_0215c3a4[];   /* layer config when the main screen is current */
extern u8 data_0215c3f4[];   /* layer config otherwise */
extern char data_0215c604[]; /* "/ohashi/res_tagbreak_black" */
extern char data_0215c620[]; /* "/ohashi/res_tagbreak_union" */
extern char data_0215c63c[]; /* "/ohashi/res_tagbreak_mix" */
extern char data_0215c658[]; /* "/ohashi/res_tagbreak" */
extern char data_0215c670[]; /* "/ohashi/res_tagbreak_head" */
extern char data_0215c68c[]; /* "/ohashi/res_tagchange" */
extern u8 gTagChangePalette[]; /* copy of the res_tagchange palette */

/* The frame counter shares its slot with Task.unk_4c. */
#define FRAME(task) (*(s16*)&(task)->unk_4c)

#define MAIN_GFX ((Unk_027c0280*)data_027c0288.unk_00)
#define STATE (data_027c027c->unk_48[data_027c0284])
#define PLAYER (&STATE->players[STATE->currentPlayer])

/* Tile data of BG `bg` on the current screen. */
#define BG_CHARS(bg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

/* Tile data of layer i of the main screen, where the sub screen copies from.
 * This code reads GfxLayer.charBase as an address, not a VRAM offset. */
#define MAIN_LAYER_CHARS(i) ((void*)GFX_LAYER(MAIN_GFX, i)->charBase)

/* First sprite tile of the extra head tiles of CO picture i (0 front, 1 back). */
#define HEAD_TILE(i) (0x5b0 + (i) * 24)

/* Task step that sets up both screens for the tag power cut-in: blending
 * and layers, a background picked by the COs' nations, the res_tagbreak
 * layers 1 and 2, the pictures of both COs, the res_tagchange layer 0 and
 * a text sprite line. Layers 1-3 of the main screen start scrolled 192
 * pixels down. Lowers the song volume and plays sound effect 81. */
void TagPowerScreen_SetUp(Task* task) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  u8* file;
  int i;

  saved0280 = data_027c0280;
  saved027c = data_027c027c;
  Screen_SetCurrentBoth(data_027c0288.unk_00);
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
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 14;
  data_027c0280->blendEvb = 4;
  /* needed to match: the original reads and writes blendEvy back unchanged */
  data_027c0280->blendEvy = *(volatile u16*)&data_027c0280->blendEvy;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 0;
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
  Screen_SetCurrentBoth(data_027c0288.unk_04);
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
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->dispCnt &= ~0x2000;
  data_027c0280->dispCnt &= ~0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->blendMode = BLEND_MODE_ALPHA;
  data_027c0280->blendEva = 14;
  data_027c0280->blendEvb = 4;
  data_027c0280->blendEvy = *(volatile u16*)&data_027c0280->blendEvy;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 0;
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
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  /* Background: res_tagbreak_black when both COs are Black Hole (in the
     * campaign: when the army is), _mix when one is, _union otherwise.
     * Needed to match: the campaign test jumps into the other branches. */
  if (STATE->gameMode == GAME_MODE_CAMPAIGN) {
    if (PLAYER->armyColour == ARMY_BLACK_HOLE) {
      goto all_black_hole;
    }
    goto no_black_hole;
  }
  if (Co_GetNation(PLAYER->front.id & PLAYER_CO_ID_MASK) == NATION_BLACK_HOLE &&
      Co_GetNation(PLAYER->back.id & PLAYER_CO_ID_MASK) == NATION_BLACK_HOLE) {
  all_black_hole:
    file = File_Load(data_0215c604);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Data_Unpack(file, BG_CHARS(data_027c0280->layer3Bg));
    Data_Unpack(file + 0x245c, data_027c0280->layer3Map);
    Palette_Load(file + 0x2a00, 32, 64);
    for (i = 0; i < 0x800; i++) {
      data_027c0280->layer3Map[i] += 0x1000;
    }
    Gfx_RequestLayerCopy(3);
    Screen_SetCurrentGfx(saved0280);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Mem_Copy32(MAIN_LAYER_CHARS(3), BG_CHARS(data_027c0280->layer3Bg), 0x6000);
    Mem_Copy32(MAIN_GFX->layer3Map, data_027c0280->layer3Map, 0x1000);
    Palette_Load(file + 0x2a00, 32, 64);
    Gfx_RequestLayerCopy(3);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(file);
  } else if (Co_GetNation(PLAYER->front.id & PLAYER_CO_ID_MASK) !=
                 NATION_BLACK_HOLE &&
             Co_GetNation(PLAYER->back.id & PLAYER_CO_ID_MASK) !=
                 NATION_BLACK_HOLE) {
  no_black_hole:
    file = File_Load(data_0215c620);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Data_Unpack(file, BG_CHARS(data_027c0280->layer3Bg));
    Data_Unpack(file + 0x2b4c, data_027c0280->layer3Map);
    Palette_Load(file + 0x3214, 32, 64);
    for (i = 0; i < 0x800; i++) {
      data_027c0280->layer3Map[i] += 0x1000;
    }
    Gfx_RequestLayerCopy(3);
    Screen_SetCurrentGfx(saved0280);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Mem_Copy32(MAIN_LAYER_CHARS(3), BG_CHARS(data_027c0280->layer3Bg), 0x6000);
    Mem_Copy32(MAIN_GFX->layer3Map, data_027c0280->layer3Map, 0x1000);
    Palette_Load(file + 0x3214, 32, 64);
    Gfx_RequestLayerCopy(3);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(file);
  } else {
    file = File_Load(data_0215c63c);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    Data_Unpack(file, BG_CHARS(data_027c0280->layer3Bg));
    Data_Unpack(file + 0x2b48, data_027c0280->layer3Map);
    Palette_Load(file + 0x31d4, 32, 64);
    for (i = 0; i < 0x800; i++) {
      data_027c0280->layer3Map[i] += 0x1000;
    }
    Gfx_RequestLayerCopy(3);
    Screen_SetCurrentGfx(saved0280);
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_04);
    Mem_Copy32(MAIN_LAYER_CHARS(3), BG_CHARS(data_027c0280->layer3Bg), 0x6000);
    Mem_Copy32(MAIN_GFX->layer3Map, data_027c0280->layer3Map, 0x1000);
    Palette_Load(file + 0x31d4, 32, 64);
    Gfx_RequestLayerCopy(3);
    Screen_SetCurrentGfx(saved0280);
    Mem_Free(file);
  }

  file = File_Load(data_0215c658);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Data_Unpack(file + 0x380, BG_CHARS(data_027c0280->layer2Bg));
  Data_Unpack(file + 0x2628, data_027c0280->layer2Map);
  Palette_Load(file + 0x2c60, 0, 32);
  Gfx_RequestLayerCopy(2);
  Data_Unpack(file + 0x2b10, BG_CHARS(data_027c0280->layer1Bg));
  Data_Unpack(file + 0x2b54, data_027c0280->layer1Map);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentGfx(saved0280);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Mem_Copy32(MAIN_LAYER_CHARS(2), BG_CHARS(data_027c0280->layer2Bg), 0x4000);
  Mem_Copy32(MAIN_GFX->layer2Map, data_027c0280->layer2Map, 0x800);
  Palette_Load(file + 0x2c60, 0, 32);
  Gfx_RequestLayerCopy(2);
  Mem_Copy32(MAIN_LAYER_CHARS(1), BG_CHARS(data_027c0280->layer1Bg), 0x100);
  Mem_Copy32(MAIN_GFX->layer1Map, data_027c0280->layer1Map, 0x800);
  Gfx_RequestLayerCopy(1);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(file);

  /* CO pictures in the colours of the current player's slot. */
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Co_LoadArmyGraphics(PLAYER->front.id, 16, STATE->currentPlayer);
  Co_LoadObjPicture(PLAYER->front.id, 0x1c0, STATE->currentPlayer);
  Co_LoadArmyPalette(PLAYER->front.id, 16, STATE->currentPlayer);
  Co_LoadArmyGraphics(PLAYER->back.id, 0x2e0, STATE->currentPlayer);
  Co_LoadObjPicture(PLAYER->back.id, 0x490, STATE->currentPlayer);
  Co_LoadArmyPalette(PLAYER->back.id, 17, STATE->currentPlayer);
  Screen_SetCurrentGfx(saved0280);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Mem_Copy32(MAIN_GFX->objVram + 0x200, data_027c0280->objVram + 0x200, 0xb400);
  Co_LoadArmyPalette(PLAYER->front.id, 16, STATE->currentPlayer);
  Co_LoadArmyPalette(PLAYER->back.id, 17, STATE->currentPlayer);
  /* Extra head tiles from res_tagbreak_head for Grit (variant 0) and
     * Kindle (variants 0 and 1); blank tiles for other COs. */
  file = File_Load(data_0215c670);
  for (i = 0; i <= 1; i++) {
    if (((&PLAYER->front)[i].id & PLAYER_CO_ID_MASK) == CO_GRIT &&
        Co_GetArmyPictureVariant((&PLAYER->front)[i].id,
                                 STATE->currentPlayer) == 0) {
      Data_Unpack(file, data_027c0280->objVram + HEAD_TILE(i) * 32);
    } else if (((&PLAYER->front)[i].id & PLAYER_CO_ID_MASK) == CO_KINDLE &&
               Co_GetArmyPictureVariant((&PLAYER->front)[i].id,
                                        STATE->currentPlayer) == 0) {
      Data_Unpack(file + 0x90, data_027c0280->objVram + HEAD_TILE(i) * 32);
    } else if (((&PLAYER->front)[i].id & PLAYER_CO_ID_MASK) == CO_KINDLE &&
               Co_GetArmyPictureVariant((&PLAYER->front)[i].id,
                                        STATE->currentPlayer) == 1) {
      Data_Unpack(file + 0x1a4, data_027c0280->objVram + HEAD_TILE(i) * 32);
    } else {
      Mem_Zero32(0, data_027c0280->objVram + HEAD_TILE(i) * 32, 0x300);
    }
  }
  Mem_Free(file);
  Screen_SetCurrentGfx(saved0280);

  file = File_Load(data_0215c68c);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  Data_Unpack(file, BG_CHARS(data_027c0280->layer0Bg));
  Data_Unpack(file + 0x28f0, data_027c0280->layer0Map);
  Palette_Load(file + 0x2cd0, 0x60, 0x60);
  Mem_Copy32(file + 0x2cd0, gTagChangePalette, 0x60);
  for (i = 0; i < 0x300; i++) {
    data_027c0280->layer0Map[i] += 0x3000;
  }
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrentGfx(saved0280);
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Mem_Copy32(MAIN_LAYER_CHARS(0), BG_CHARS(data_027c0280->layer0Bg), 0x6000);
  Data_Unpack(file + 0x24dc, data_027c0280->layer0Map);
  Palette_Load(file + 0x2cd0, 0x60, 0x60);
  for (i = 0; i < 0x300; i++) {
    data_027c0280->layer0Map[i] += 0x3000;
  }
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrentGfx(saved0280);
  Mem_Free(file);

  if (Link_IsActive() == 1) {
    SpriteTask_Create(232, 8, 0, 15);
  }
  task->unk_58 = BreakText_LoadChangeFont(Text_Get(0x7d00001d), 0x61a, 2);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 192;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 192;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 192;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  Sound_LowerSongVolume(101);
  Sound_PlaySe(81);
  FRAME(task) = 0;
}
