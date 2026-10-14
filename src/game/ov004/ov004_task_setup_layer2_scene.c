#include "types.h"
#include "game/Unk_ov004_022bf158.h"
#include "game/screen_other_map.h"

void Screen_InitLayers(const GfxLayer* config);
void Screen_SetBgMode(int bgMode);
void Gfx_ClearLayerMap(int layer);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);
void Ov004_SetupOrtho3d(void);
void Ov004_ClearSlots(Unk_ov004_022bf158* state);
void Ov004_UnpackResource(const char* name, void* dest);
void Ov004_CopyResource(const char* name, void* dest);
void Ov004_LoadObjGraphics(Unk_ov004_022bf158* state);
void func_ov004_022ba964(Unk_ov004_022bf158* state);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern GfxLayer data_ov004_022be908[];
extern char data_ov004_022bdd88[]; /* layer 2 tiles */
extern char data_ov004_022bddf8[]; /* layer 2 map */
extern char data_ov004_022bde0c[]; /* palette */
extern u32 data_ov004_022bdc94[];  /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84
    []; /* map bytes of a rotation/scaling BG by screen size */

/* Size in bytes of the tile map of a layer (as in Ov004_LoadBgLayers1And2). */
#define LAYER_MAP_BYTES(layer, i)                                    \
  (Screen_IsTextBg(i)     ? data_ov004_022bdc94[(layer)->screenSize] \
   : Screen_IsAffineBg(i) ? data_ov004_022bdc84[(layer)->screenSize] \
                          : data_ov004_022bdc84[(layer)->screenSize] * 2)

/* Script step: sets up the main screen with every BG and OBJ hidden and
 * loads layer 2's tiles, map and palette. */
void Ov004Task_SetupLayer2Scene(Ov004Task* task) {
  Unk_ov004_022bf158* state = data_ov004_022bf158;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedScreen;
  u16* map;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedScreen = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_SetBgMode(0);
  Screen_InitLayers(data_ov004_022be908);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt &= ~0x400;
  data_027c0280->dispCnt &= ~0x800;
  data_027c0280->dispCnt &= ~0x1000;
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 3;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 2;
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
  Gfx_ClearLayerMap(2);

  Ov004_UnpackResource(
      data_ov004_022bdd88,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer2Bg].charBase << 14));
  Ov004_UnpackResource(data_ov004_022bddf8, GFX_LAYER(data_027c0280, 2)->map);
  Ov004_CopyResource(data_ov004_022bde0c, data_027c0280->paletteBuffer);
  map = GFX_LAYER(data_027c0280, 2)->map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 2), 2), 0);
  Gfx_RequestLayerCopy(2);
  Palette_Flush();
  Ov004_LoadObjGraphics(state);
  func_ov004_022ba964(state);
  Screen_SetCurrent(savedScreen);
  Screen_SetCurrentGfx(savedGfx);

  Ov004_SetupOrtho3d();
  state->unk_8f8 = 0;
  Ov004_ClearSlots(state);
  task->frame = 0;
  task->duration = 0;
  task->step = 0;
}
