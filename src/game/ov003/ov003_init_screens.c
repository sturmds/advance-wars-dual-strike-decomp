#include "types.h"
#include "sdk/gx.h"
#include "game/screen_other_map.h"

void Screen_SetCurrentBoth(void *state);
void Screen_InitLayers(const GfxLayer *config);
void Screen_SetBgMode(int bgMode);
void Screen_SetBg0Mode(int bg0Is3d);   /* sets Unk_027c0280.bg0Mode and applies the graphics mode */
void func_0210ba04(void);          /* SDK 3D engine init (G3X_Init?) */
void func_0210bd70(int top, int bottom, int left, int right, int near, int far, int scaleW, int draw, int mtx);   /* SDK orthographic projection (G3i_OrthoW_) */

extern GfxLayer Ov003_gMainLayerConfig[4];
extern GfxLayer Ov003_gSubLayerConfig[4];

/* Sets up both screens for overlay 3 and leaves them black, ready to fade
 * in. Main screen: BG0 shows the 3D engine with an orthographic 256x192
 * projection, layers 0 and 3 show inside the OBJ window. Sub screen: layer 1
 * alpha-blends over the sprites (4/16 over 12/16). */
void Ov003_InitScreens(void)
{
    Unk_027c0280 *saved0280;
    Unk_02080440 *saved027c;

    Screen_SetCurrentBoth(data_027c0288.unk_00);
    func_0210ba04();
    Screen_InitLayers(Ov003_gMainLayerConfig);
    *REG_DISP3DCNT &= 0xcfef;          /* anti-aliasing off, acknowledge the overflow flags */
    *REG_SWAP_BUFFERS = 1;             /* manual polygon sort, Z buffering */
    G3X_SetClearColor(0x2bea, 0, 0x7fff, 63, FALSE);
    *REG_VIEWPORT = 0xbfff0000;        /* (0, 0) to (255, 191) */
    func_0210bd70(0, 192, 0, 256, 1, 0x1000, 0x1000, 1, 0);
    *REG_MTX_STORE = 0;
    *REG_DISPCNT &= ~0x07000000;       /* BG character base offset 0 */
    *REG_DISPCNT &= ~0x38000000;       /* BG screen base offset 0 */
    Screen_SetBgMode(0);
    Screen_SetBg0Mode(1);
    data_027c0280->dispCnt |= 0x100;   /* BG0-BG3 and OBJ on */
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->dispCnt |= 0x8000;  /* OBJ window on */
    data_027c0280->objWinIn.bg[data_027c0280->layer0Bg] = 1;
    data_027c0280->objWinIn.bg[data_027c0280->layer1Bg] = 0;
    data_027c0280->objWinIn.bg[data_027c0280->layer2Bg] = 0;
    data_027c0280->objWinIn.bg[data_027c0280->layer3Bg] = 1;
    data_027c0280->objWinIn.obj = 1;
    data_027c0280->winOut.bg[data_027c0280->layer0Bg] = 1;
    data_027c0280->winOut.bg[data_027c0280->layer1Bg] = 1;
    data_027c0280->winOut.bg[data_027c0280->layer2Bg] = 1;
    data_027c0280->winOut.bg[data_027c0280->layer3Bg] = 1;
    data_027c0280->winOut.obj = 1;
    data_027c0280->objWinIn.effect = 1;
    data_027c0280->winOut.effect = 1;
    data_027c0280->blendMode = 0;
    data_027c0280->blendEva = 16;
    data_027c0280->blendEvb = 0;
    data_027c0280->blendEvy = 0;
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;

    Screen_SetCurrentBoth(data_027c0288.unk_04);
    Screen_InitLayers(Ov003_gSubLayerConfig);
    Screen_SetBgMode(0);
    data_027c0280->dispCnt |= 0x100;
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 1;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
    data_027c0280->blendMode = 1;
    data_027c0280->blendEva = 4;
    data_027c0280->blendEvb = 12;
    data_027c0280->blendEvy = 0;
    data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 1;
    data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendTopObj = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 0;
    data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 0;
    data_027c0280->blendBottomObj = 1;
    data_027c0280->bgScrollY[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer0Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer1Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer2Bg] = 0;
    data_027c0280->bgScrollY[data_027c0280->layer3Bg] = 0;
    data_027c0280->bgScrollX[data_027c0280->layer3Bg] = 0;
    data_027c0280->dispCnt |= 0x40;
    data_027c0280->unk_dd = 1;

    saved0280 = data_027c0280;
    saved027c = data_027c027c;
    Screen_SetCurrentBoth(data_027c0288.unk_00);
    data_027c0280->masterBrightness = -16;
    Screen_SetCurrentBoth(data_027c0288.unk_04);
    data_027c0280->masterBrightness = -16;
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
}
