#include "types.h"
#include "game/screen_other_map.h"

void Screen_SetLayerScreenSize(int layer, int screenSize);
void Gfx_ClearLayerMap(int layer);
void Gfx_RequestLayerCopy(int layer);

/* Task script step before the day banner opens: windows off, and layer 0
 * alpha-blended over layers 1-3, the sprites and the backdrop, starting fully
 * transparent (EVA 0, EVB 16). DayBanner_OpenUpdate then raises EVA. */
void DayBanner_SetUpBlend(void)
{
    GFX_DISPCNT(data_027c0280)->win0 = 0;
    GFX_DISPCNT(data_027c0280)->win1 = 0;
    GFX_DISPCNT(data_027c0280)->objWin = 0;
    data_027c0280->blendMode = BLEND_MODE_ALPHA;
    data_027c0280->blendEva = 0;
    data_027c0280->blendEvb = 16;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendTopBackdrop = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
    data_027c0280->blendBottomObj = 1;
    data_027c0280->blendBottomBackdrop = 1;
    Screen_SetLayerScreenSize(0, 1);
    Gfx_ClearLayerMap(0);
    Gfx_RequestLayerCopy(0);
}
