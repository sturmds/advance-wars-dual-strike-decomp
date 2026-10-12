#include "types.h"
#include "game/BgControl.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

#define LAYER_BG(i) (GFX_LAYER(data_027c0280, i)->bg)

/* Gives the four layers' BGs priorities 0 to 3, shows all BGs and OBJ, turns
 * blending off (EVA 16, EVB and EVY 0), selects 1D OBJ tile mapping and
 * clears the scroll of every layer. */
void Screen_SetLayerDefaults(void) {
  data_027c0280->bgCnt[LAYER_BG(0)].priority = 0;
  data_027c0280->bgCnt[LAYER_BG(1)].priority = 1;
  data_027c0280->bgCnt[LAYER_BG(2)].priority = 2;
  data_027c0280->bgCnt[LAYER_BG(3)].priority = 3;
  GFX_DISPCNT(data_027c0280)->bg0 = 1;
  GFX_DISPCNT(data_027c0280)->bg1 = 1;
  GFX_DISPCNT(data_027c0280)->bg2 = 1;
  GFX_DISPCNT(data_027c0280)->bg3 = 1;
  GFX_DISPCNT(data_027c0280)->obj = 1;
  data_027c0280->blendMode = 0;
  data_027c0280->blendEva = 16;
  data_027c0280->blendEvb = 0;
  data_027c0280->blendEvy = 0;
  GFX_DISPCNT(data_027c0280)->objMap1D = 1;
  data_027c0280->unk_dd = 0;
  data_027c0280->bgScrollY[LAYER_BG(0)] = 0;
  data_027c0280->bgScrollX[LAYER_BG(0)] = 0;
  data_027c0280->bgScrollY[LAYER_BG(1)] = 0;
  data_027c0280->bgScrollX[LAYER_BG(1)] = 0;
  data_027c0280->bgScrollY[LAYER_BG(2)] = 0;
  data_027c0280->bgScrollX[LAYER_BG(2)] = 0;
  data_027c0280->bgScrollY[LAYER_BG(3)] = 0;
  data_027c0280->bgScrollX[LAYER_BG(3)] = 0;
}
