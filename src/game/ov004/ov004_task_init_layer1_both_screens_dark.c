#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void TileMap_AddToEntries(u16* entries, u16 byteCount, int addend);
BOOL Screen_IsTextBg(int layer);
BOOL Screen_IsAffineBg(int layer);
void Gfx_RequestLayerCopy(int layer);
void Palette_Flush(void);
void Ov004_UnpackResource(const char* name,
                          void* dest); /* unpacks a named file */
void Ov004_CopyResource(const char* name, void* dest); /* copies a named file */
void Ov004_ClearSlots(Unk_ov004_022bf158* state);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern char data_ov004_022bdd44[]; /* "06f": layer 1 tiles */
extern char data_ov004_022bddb4[]; /* "071": layer 1 map of the main screen */
extern char data_ov004_022bde24[]; /* "072": layer 1 map of the other screen */
extern char data_ov004_022bddb8[]; /* "070": palette */
extern u32 data_ov004_022bdc94[];  /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84
    []; /* map bytes of a rotation/scaling BG by screen size */

/* Size in bytes of the tile map of a layer (as in Gfx_LoadBmap083). */
#define LAYER_MAP_BYTES(layer, i)                                    \
  (Screen_IsTextBg(i)     ? data_ov004_022bdc94[(layer)->screenSize] \
   : Screen_IsAffineBg(i) ? data_ov004_022bdc84[(layer)->screenSize] \
                          : data_ov004_022bdc84[(layer)->screenSize] * 2)

/* Only BG1 on, priorities 0 to 3, no windows, blending or scroll. Then a
 * picture in layer 1: tiles 06f, the given map and palette 070. */
#define SHOW_LAYER1_PICTURE(mapName)                                           \
  do {                                                                         \
    GFX_DISPCNT(data_027c0280)->bg0 = 0;                                       \
    GFX_DISPCNT(data_027c0280)->bg1 = 1;                                       \
    GFX_DISPCNT(data_027c0280)->bg2 = 0;                                       \
    GFX_DISPCNT(data_027c0280)->bg3 = 0;                                       \
    GFX_DISPCNT(data_027c0280)->obj = 0;                                       \
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;                \
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;                \
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;                \
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;                \
    GFX_DISPCNT(data_027c0280)->win0 = 0;                                      \
    GFX_DISPCNT(data_027c0280)->win1 = 0;                                      \
    GFX_DISPCNT(data_027c0280)->objWin = 0;                                    \
    data_027c0280->blendMode = 0;                                              \
    data_027c0280->blendEva = 16;                                              \
    data_027c0280->blendEvb = 0;                                               \
    data_027c0280->blendEvy = 0;                                               \
    GFX_DISPCNT(data_027c0280)->objMap1D = 1;                                  \
    data_027c0280->unk_dd = 0;                                                 \
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;                     \
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;                     \
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;                     \
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;                     \
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;                     \
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;                     \
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;                     \
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;                     \
    Ov004_UnpackResource(                                                      \
        data_ov004_022bdd44,                                                   \
        data_027c0280->bgVram +                                                \
            (data_027c0280->bgCnt[data_027c0280->layer1Bg].charBase << 14));   \
    Ov004_UnpackResource(mapName, data_027c0280->layer1Map);                   \
    map = data_027c0280->layer1Map;                                            \
    TileMap_AddToEntries(map, LAYER_MAP_BYTES(GFX_LAYER(data_027c0280, 1), 1), \
                         0);                                                   \
    Gfx_RequestLayerCopy(1);                                                   \
    Ov004_CopyResource(data_ov004_022bddb8, data_027c0280->paletteBuffer);     \
    Palette_Flush();                                                           \
  } while (0)

/* Script step: shows the same picture in layer 1 of both screens, with map
 * 071 on the main screen and 072 on the other, and starts both screens at
 * full black brightness. */
void Ov004Task_InitLayer1BothScreensDark(Ov004Task* task) {
  Unk_ov004_022bf158* state;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;
  u16* map;

  state = data_ov004_022bf158;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  SHOW_LAYER1_PICTURE(data_ov004_022bddb4);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  SHOW_LAYER1_PICTURE(data_ov004_022bde24);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->masterBrightness = -16;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->masterBrightness = -16;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  Ov004_ClearSlots(state);
  state->unk_810 = NULL;
  state->unk_814 = NULL;
  task->step = 0;
}
