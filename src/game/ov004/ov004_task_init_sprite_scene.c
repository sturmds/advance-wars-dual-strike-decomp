#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov004_022bf158.h"

void Screen_InitLayers(const void* config);
void Gfx_RequestLayerCopy(int layer);
void Ov004_UnpackResource(const char* file,
                          void* dest); /* unpacks a file, see FILE_ID */
void Ov004_CopyResource(const char* file,
                        void* dest); /* copies a file, see FILE_ID */
/* Stores its arguments into state->bg3Params, in field order. */
void Ov004_ClearSlots(Unk_ov004_022bf158* state);

extern Unk_ov004_022bf158* data_ov004_022bf158;
extern GfxLayer data_ov004_022be868[]; /* main screen layer config */
extern char data_ov004_022bdd3c[];     /* "079": layer 3 tiles */
extern char data_ov004_022bdd80[];     /* "07b": layer 3 map */
extern char data_ov004_022bddd0[];     /* "07a": palette */

/* The file helpers take a name, or a number below 0x10000 in place of the
 * name pointer. */
#define FILE_ID(id) ((const char*)(id))

/* Script step: sets up the main screen with BG0 and OBJ on, priorities 0
 * to 3, no windows, blending or scroll, and normal brightness. Loads two
 * sprite tile sets with their palettes (0x1015 and 0x1014, OBJ palettes 0
 * and 1), a picture into layer 3 (079, 07b) and the BG palette (07a). Then
 * sets bg3Params and the eased values to their start. */
void Ov004Task_InitSpriteScene(Ov004Task* task) {
  Unk_ov004_022bf158* state;
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  state = data_ov004_022bf158;
  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Screen_InitLayers(data_ov004_022be868);
  GFX_DISPCNT(data_027c0280)->bg0 = 1;
  GFX_DISPCNT(data_027c0280)->bg1 = 0;
  GFX_DISPCNT(data_027c0280)->bg2 = 0;
  GFX_DISPCNT(data_027c0280)->bg3 = 0;
  GFX_DISPCNT(data_027c0280)->obj = 1;
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

  Ov004_CopyResource(FILE_ID(0x1015), data_027c0280->objVram);
  Ov004_CopyResource(FILE_ID(0x2015), data_027c0280->paletteBuffer + 0x200);
  Ov004_CopyResource(FILE_ID(0x1014), data_027c0280->objVram + 0x4000);
  Ov004_CopyResource(FILE_ID(0x2014), data_027c0280->paletteBuffer + 0x220);
  Ov004_UnpackResource(
      data_ov004_022bdd3c,
      data_027c0280->bgVram +
          (data_027c0280->bgCnt[data_027c0280->layer3Bg].charBase << 14));
  Ov004_UnpackResource(data_ov004_022bdd80, data_027c0280->layer3Map);
  Gfx_RequestLayerCopy(3);
  Ov004_CopyResource(data_ov004_022bddd0, data_027c0280->paletteBuffer);
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->masterBrightness = 0;
  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);

  Ov004_SetBg3Params(state, 0x60, 0x60, 0x160, 0x60, 0x100, 0x100, 0);
  Ov004_ClearSlots(state);
  state->unk_8f8 = 0x100;
  state->unk_8fc = 0x100;
  state->unk_900 = 0;
  state->unk_904 = 0;
  state->unk_908 = 0x180;
  state->unk_90c = 0x180;
  task->frame = 0;
  task->duration = 0;
  task->step = 0;
}
