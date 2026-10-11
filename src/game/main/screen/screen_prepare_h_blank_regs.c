#include "types.h"
#include "sdk/gx.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

/* Main engine register values that the H-blank handlers
 * (Screen_HBlankTopPanel, Screen_HBlankBottomPanel) write during the frame. */
extern u32 data_027c0270;  /* DISPCNT BG/OBJ layer bits */
extern u32 data_027c026c;  /* DISPCNT window bits */
extern u32 data_027c0264;  /* BG1OFS */
extern u32 data_027c0274;  /* BG2OFS */
extern u16 data_027c0254;  /* BG1CNT */
extern u16 data_027c0258;  /* BG2CNT */
extern s16 data_027c025c;  /* BG2PA */
extern s16 data_027c0260;  /* BG2PB */
extern s16 data_027c023c;  /* BG2PC */
extern s16 data_027c0248;  /* BG2PD */
extern s16 data_027c0250;  /* BG2X */
extern s16 data_027c0244;  /* BG2Y */
extern u32 data_027c0268;  /* BLDCNT */
extern u8 data_027c0238;

BOOL Screen_IsTextBg(int bg);
void func_0210b580(u32 *bldcnt, int plane1, int plane2, int ev1, int ev2);
void func_0210b528(u32 *bldcnt, int plane1, int plane2, int ev1, int ev2, int brightness);

#define GFX data_027c0280

/* Needed to match: the scroll values are read through a volatile view
 * and passed to an inline function, so the compiler loads Y before X
 * while the expression keeps X first. */
#define VGFX ((volatile Unk_027c0280 *)data_027c0280)

/* BGxOFS value: x in bits 0-8, y in bits 16-24. */
static inline u32 BgOffset(int x, int y)
{
    return (x & 0x1ff) | ((y << 16) & 0x1ff0000);
}

#define BG_PLANES(bg, obj) \
    (((bg)[0] ? 1 : 0) | ((bg)[1] ? 2 : 0) | ((bg)[2] ? 4 : 0) | ((bg)[3] ? 8 : 0) | ((obj) ? 16 : 0))
#define BLEND_TOP_PLANES (BG_PLANES(GFX->blendTopBg, GFX->blendTopObj) | (GFX->blendTopBackdrop ? 32 : 0))
#define BLEND_BOTTOM_PLANES (BG_PLANES(GFX->blendBottomBg, GFX->blendBottomObj) | (GFX->blendBottomBackdrop ? 32 : 0))

/* Packs the current screen state's BG1/BG2, layer, window and blend
 * settings into main engine register values for the H-blank handler. */
void Screen_PrepareHBlankRegs(void)
{
    data_027c0270 = (GFX_DISPCNT(GFX)->bg0 ? 1 : 0) | (GFX_DISPCNT(GFX)->bg1 ? 2 : 0) | (GFX_DISPCNT(GFX)->bg2 ? 4 : 0) |
                    (GFX_DISPCNT(GFX)->bg3 ? 8 : 0) | (GFX_DISPCNT(GFX)->obj ? 16 : 0);
    data_027c026c = (GFX_DISPCNT(GFX)->win0 ? 1 : 0) | (GFX_DISPCNT(GFX)->win1 ? 2 : 0) | (GFX_DISPCNT(GFX)->objWin ? 4 : 0);

    data_027c0264 = BgOffset(VGFX->bgScrollX[1], VGFX->bgScrollY[1]);
    data_027c0274 = BgOffset(VGFX->bgScrollX[2], VGFX->bgScrollY[2]);

    data_027c0254 = (GFX->bgCnt[1].screenSize << 14) | (*REG_BG1CNT & 0x40) | (GFX->bgCnt[1].colorMode << 7) |
                    (GFX->bgCnt[1].screenBase << 8) | (GFX->bgCnt[1].charBase << 2) | GFX->bgCnt[1].priority;

    if (Screen_IsTextBg(2) == TRUE) {
        data_027c0258 = (GFX->bgCnt[2].screenSize << 14) | (*REG_BG2CNT & 0x40) | (GFX->bgCnt[2].colorMode << 7) |
                        (GFX->bgCnt[2].screenBase << 8) | (GFX->bgCnt[2].charBase << 2) | GFX->bgCnt[2].priority;
    } else {
        data_027c0258 = (GFX->bgCnt[2].screenSize << 14) | (*REG_BG2CNT & 0x40) | (GFX->bgCnt[2].screenBase << 8) |
                        (GFX->bgCnt[2].charBase << 2) | (GFX->bgCnt[2].extPltt << 13) | GFX->bgCnt[2].priority;
        data_027c025c = GFX->bg2Affine.pa;
        data_027c0260 = GFX->bg2Affine.pb;
        data_027c023c = GFX->bg2Affine.pc;
        data_027c0248 = GFX->bg2Affine.pd;
        data_027c0250 = GFX->bg2Affine.x;
        data_027c0244 = GFX->bg2Affine.y;
    }

    switch (GFX->blendMode) {
    default:
    case 0:
        data_027c0268 = 0;
        break;
    case 1:
        func_0210b580(&data_027c0268, BLEND_TOP_PLANES, BLEND_BOTTOM_PLANES, GFX->blendEva, GFX->blendEvb);
        break;
    case 2:
        func_0210b528(&data_027c0268, BLEND_TOP_PLANES, BLEND_BOTTOM_PLANES, GFX->blendEva, GFX->blendEvb,
                      GFX->blendEvy);
        break;
    case 3:
        func_0210b528(&data_027c0268, BLEND_TOP_PLANES, BLEND_BOTTOM_PLANES, GFX->blendEva, GFX->blendEvb,
                      -GFX->blendEvy);
        break;
    }

    data_027c0238 = GFX->bgScreenOffset;
}
