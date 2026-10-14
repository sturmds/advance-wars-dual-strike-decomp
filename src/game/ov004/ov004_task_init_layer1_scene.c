#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void Screen_InitLayers(const void* config);
void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);
void Mem_Copy32(const void* src, void* dst, u32 size);
void Mem_Zero32(u32 value, void* dst, u32 size);
void Ov004_UnpackResource(const char* name,
                          void* dest); /* unpacks a named file */
void Ov004_CopyResource(const char* name, void* dest); /* copies a named file */
void Ov004_ClearSlots(Unk_ov004_022bf158* state);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern GfxLayer data_ov004_022be868[]; /* main screen layer config */
extern char data_ov004_022bdd44[];     /* "06f": layer 1 tiles */
extern char data_ov004_022bdd5c[];     /* "074": layer 1 map */
extern char data_ov004_022bddb8[];     /* "070": palette */
extern char data_ov004_022bdd58[];     /* "075": layer 3 tiles */
extern char data_ov004_022bdd50[];     /* "076": layer 3 map */
extern u32 data_ov004_022bdc94[]; /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84
    []; /* map bytes of a rotation/scaling BG by screen size */

/* Size in bytes of the tile map of a layer (as in Gfx_LoadBmap083). */
#define LAYER_MAP_BYTES(layer, i)                                    \
  (Screen_IsTextBg(i)     ? data_ov004_022bdc94[(layer)->screenSize] \
   : Screen_IsAffineBg(i) ? data_ov004_022bdc84[(layer)->screenSize] \
                          : data_ov004_022bdc84[(layer)->screenSize] * 2)

/* Script step: sets up the main screen with only BG1 on, priorities 0 to
 * 3, no windows, blending or scroll. Loads a picture into layer 1 (06f,
 * 074) with palette 070 in BG palette 1, keeps the palettes in the state
 * and clears BG palette 1 (likely for a fade in). Loads a second picture
 * into layer 3 (075, 076) with the same palette in BG palette 0. */
void Ov004Task_InitLayer1Scene(Ov004Task* task) {
  Unk_ov004_022bf158* state;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  u16* map;

  state = data_ov004_022bf158;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_InitLayers(data_ov004_022be868);
  GFX_DISPCNT(data_027c0280)->bg0 = 0;
  GFX_DISPCNT(data_027c0280)->bg1 = 1;
  GFX_DISPCNT(data_027c0280)->bg2 = 0;
  GFX_DISPCNT(data_027c0280)->bg3 = 0;
  GFX_DISPCNT(data_027c0280)->obj = 0;
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  GFX_DISPCNT(data_027c0280)->win0 = 0;
  GFX_DISPCNT(data_027c0280)->win1 = 0;
  GFX_DISPCNT(data_027c0280)->objWin = 0;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  GFX_DISPCNT(data_027c0280)->objMap1D = 1;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
  data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
  data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;

  Ov004_UnpackResource(
      data_ov004_022bdd44,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14));
  Ov004_UnpackResource(data_ov004_022bdd5c, data_027c0280->layer1Map);
  map = data_027c0280->layer1Map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 1), 1),
                       0x1000);
  Gfx_RequestLayerCopy(1);
  Ov004_CopyResource(data_ov004_022bddb8, data_027c0280->paletteBuffer + 0x20);
  Mem_Copy32(data_027c0280->paletteBuffer, state->savedPalette,
             sizeof(state->savedPalette));
  Mem_Zero32(0, data_027c0280->paletteBuffer + 0x20, 0x20);
  Ov004_UnpackResource(
      data_ov004_022bdd58,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
  Ov004_UnpackResource(data_ov004_022bdd50, data_027c0280->layer3Map);
  Ov004_CopyResource(data_ov004_022bddb8, data_027c0280->paletteBuffer);
  Palette_Flush();
  Gfx_RequestLayerCopy(3);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  Ov004_ClearSlots(state);
  task->frame = 0;
  task->duration = 0;
  task->tweenFrames = 0;
  task->step = 0;
}
