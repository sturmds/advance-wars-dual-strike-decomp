#include "types.h"
#include "game/menu_button.h"
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
void Ov004_UnpackResource(const char* name, void* dest);
void Ov004_CopyResource(const char* name, void* dest);
void Ov004_ClearSlots(Unk_ov004_022bf158* state);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern u8 data_ov004_022be868[]; /* main screen layer config */
extern const MenuButtonTemplate data_ov004_022be840[];
extern char data_ov004_022bdd40[];
extern char data_ov004_022bdd44[]; /* "06f": layer 1 tiles */
extern char data_ov004_022bdd4c[];
extern char data_ov004_022bddb0[];
extern char data_ov004_022bddb8[]; /* "070": palette */
extern char data_ov004_022bddcc[];
extern char data_ov004_022bddd8[];
extern char data_ov004_022bde28[];
extern u32 data_ov004_022bdc94[]; /* map bytes of a text BG by screen size */
extern u32 data_ov004_022bdc84
    []; /* map bytes of a rotation/scaling BG by screen size */

#define OV004_BUTTON_ID 0xfd

/* Byte size of layer i's tile map. */
#define LAYER_MAP_BYTES(i)                                            \
  (Screen_IsTextBg(i)                                                 \
       ? data_ov004_022bdc94[GFX_LAYER(data_027c0280, i)->screenSize] \
   : Screen_IsAffineBg(i)                                             \
       ? data_ov004_022bdc84[GFX_LAYER(data_027c0280, i)->screenSize] \
       : data_ov004_022bdc84[GFX_LAYER(data_027c0280, i)->screenSize] * 2)

/* Tile data of BG bg (BG VRAM plus its character base). */
#define BG_CHAR_DATA(bg) \
  (data_027c0280->bgVram + (data_027c0280->bgCnt[bg].charBase << 14))

/* Priorities 0 to 3, no windows, blending or scroll. */
#define RESET_LAYERS()                                          \
  do {                                                          \
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0; \
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1; \
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2; \
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3; \
    GFX_DISPCNT(data_027c0280)->win0 = 0;                       \
    GFX_DISPCNT(data_027c0280)->win1 = 0;                       \
    GFX_DISPCNT(data_027c0280)->objWin = 0;                     \
    data_027c0280->blendMode = 0;                               \
    data_027c0280->blendEva = 16;                               \
    data_027c0280->blendEvb = 0;                                \
    data_027c0280->blendEvy = 0;                                \
    GFX_DISPCNT(data_027c0280)->objMap1D = 1;                   \
    data_027c0280->unk_dd = 0;                                  \
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;      \
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;      \
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;      \
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;      \
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;      \
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;      \
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;      \
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;      \
  } while (0)

/* Script step: sets up the main screen with BG1 and BG2 on and loads a
 * picture into layer 1 (06f tiles, palette 070 in BG palette 0), keeps the
 * palettes in the state and clears them (likely for a fade in). Then sets up
 * the other screen with BG0 and OBJ on, loads layer 0, sprite tiles and an OBJ
 * palette, and adds menu button 253 there. */
void Ov004Task_InitButtonScene(Ov004Task* task) {
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
  GFX_DISPCNT(data_027c0280)->bg2 = 1;
  GFX_DISPCNT(data_027c0280)->bg3 = 0;
  GFX_DISPCNT(data_027c0280)->obj = 0;
  RESET_LAYERS();
  Ov004_UnpackResource(data_ov004_022bdd44,
                       BG_CHAR_DATA(data_027c0280->layer1Bg));
  Ov004_UnpackResource(data_ov004_022bdd40, data_027c0280->layer1Map);
  map = data_027c0280->layer1Map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(1), 0);
  Gfx_RequestLayerCopy(1);
  Ov004_CopyResource(data_ov004_022bddb8, data_027c0280->paletteBuffer);
  Palette_Flush();
  Mem_Copy32(data_027c0280->paletteBuffer, state->savedPalette, 0x400);
  Mem_Zero32(0, data_027c0280->paletteBuffer, 0x400);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  GFX_DISPCNT(data_027c0280)->bg0 = 1;
  GFX_DISPCNT(data_027c0280)->bg1 = 0;
  GFX_DISPCNT(data_027c0280)->bg2 = 0;
  GFX_DISPCNT(data_027c0280)->bg3 = 0;
  GFX_DISPCNT(data_027c0280)->obj = 1;
  RESET_LAYERS();
  Ov004_UnpackResource(data_ov004_022bde28,
                       BG_CHAR_DATA(data_027c0280->layer0Bg));
  Ov004_UnpackResource(data_ov004_022bddd8, data_027c0280->layer0Map);
  map = data_027c0280->layer0Map;
  TileMap_AddToEntries(map, LAYER_MAP_BYTES(0), 0);
  Gfx_RequestLayerCopy(0);
  Ov004_CopyResource(data_ov004_022bddb0, data_027c0280->paletteBuffer);
  Ov004_UnpackResource(data_ov004_022bdd4c, data_027c0280->objVram);
  Ov004_CopyResource(data_ov004_022bddcc, data_027c0280->paletteBuffer + 0x200);
  Palette_Flush();
  MenuButton_Add(0, 0, data_ov004_022be840, OV004_BUTTON_ID);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  Ov004_ClearSlots(state);
  task->frame = 0;
  task->duration = 0;
  task->step = 0;
}
