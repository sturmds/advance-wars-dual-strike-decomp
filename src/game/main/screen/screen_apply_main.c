#include "types.h"
#include "game/screen_shake.h"
#include "sdk/gx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

void Screen_SetCurrent(Unk_02080440* state);
void Screen_SetCurrentGfx(Unk_027c0280* state);
BOOL Screen_IsTextBg(int bg);
void GX_VBlankIntr(BOOL enable);
void GX_HBlankIntr(BOOL enable);
void func_0210a5d4(int line);
void G2x_SetBlendBrightnessExt(volatile u16* reg, int top, int bottom, int eva,
                               int evb, int evy);
void G2x_SetBlendAlpha(volatile u16* reg, int top, int bottom, int eva,
                       int evb);
void func_0210b684(int offset);

#define GFX data_027c0280

/* The register writes below (DISPCNT, BGxCNT, BGxOFS) go through inline
 * functions. The original's instruction order only comes out when the
 * writes go through inline functions like these. */

static inline void SetVisiblePlane(int planes) {
  *REG_DISPCNT = (*REG_DISPCNT & ~0x1f00) | (planes << 8);
}

static inline void SetVisibleWnd(int windows) {
  *REG_DISPCNT = (*REG_DISPCNT & ~0xe000) | (windows << 13);
}

static inline void SetObjVramModeChar(int mode) {
  *REG_DISPCNT = (*REG_DISPCNT & ~0x300010) | mode;
}

static inline void SetObjHBlankProc(BOOL on) {
  if (on) {
    *REG_DISPCNT |= 0x800000;
  } else {
    *REG_DISPCNT &= ~0x800000;
  }
}

static inline void SetVCountIrq(BOOL on) {
  if (on) {
    *REG_DISPSTAT |= 0x20;
  } else {
    *REG_DISPSTAT &= ~0x20;
  }
}

static inline void SetBgOffset(volatile u32* reg, int x, int y) {
  *reg = (x & 0x1ff) | ((y << 16) & 0x1ff0000);
}

static inline void SetBgScreenOffset(int offset) {
  *REG_DISPCNT = (offset << 27) | (*REG_DISPCNT & ~0x38000000);
}

/* Needed to match: the screen size term comes before the register read. */
static inline void SetBgControlText(volatile u16* reg, int screenSize,
                                    int colorMode, int screenBase,
                                    int charBase) {
  *reg = (screenSize << 14) | (*reg & 0x43) | (colorMode << 7) |
         (screenBase << 8) | (charBase << 2);
}

static inline void SetBgControlAffine(volatile u16* reg, int screenSize,
                                      int areaOver, int screenBase,
                                      int charBase) {
  *reg = (screenSize << 14) | (*reg & 0x43) | (screenBase << 8) |
         (charBase << 2) | (areaOver << 13);
}

static inline void SetBgPriority(volatile u16* reg, int priority) {
  *reg = (*reg & ~3) | priority;
}

static inline void SetBgMosaic(volatile u16* reg, BOOL on) {
  if (on) {
    *reg |= 0x40;
  } else {
    *reg &= ~0x40;
  }
}

static inline void SetMosaicSize(volatile u8* reg, int h, int v) {
  *reg = h | (v << 4);
}

static inline void SetBlendAlpha(int top, int bottom, int eva, int evb) {
  G2x_SetBlendAlpha(REG_BLDCNT, top, bottom, eva, evb);
}

static inline void SetBlendBrightness(int top, int bottom, int eva, int evb,
                                      int evy) {
  G2x_SetBlendBrightnessExt(REG_BLDCNT, top, bottom, eva, evb, evy);
}

static inline void SetWnd0Position(int left, int top, int right, int bottom) {
  *REG_WIN0H = ((left << 8) & 0xff00) | (right & 0xff);
  *REG_WIN0V = ((top << 8) & 0xff00) | (bottom & 0xff);
}

static inline void SetWnd1Position(int left, int top, int right, int bottom) {
  *REG_WIN1H = ((left << 8) & 0xff00) | (right & 0xff);
  *REG_WIN1V = ((top << 8) & 0xff00) | (bottom & 0xff);
}

static inline void SetWndPlaneLow(volatile u16* reg, int planes, BOOL effect) {
  u32 value = (*reg & ~0x3f) | planes;
  if (effect) {
    value |= 0x20;
  }
  *reg = value;
}

static inline void SetWndPlaneHigh(volatile u16* reg, int planes, BOOL effect) {
  u32 value = (*reg & ~0x3f00) | (planes << 8);
  if (effect) {
    value |= 0x2000;
  }
  *reg = value;
}

/* Layer bits as WININ, WINOUT and BLDCNT take them. */
#define BG_PLANES(bg)                                          \
  (((bg)[0] ? 1 : 0) | ((bg)[1] ? 2 : 0) | ((bg)[2] ? 4 : 0) | \
   ((bg)[3] ? 8 : 0))

#define BLEND_TOP_PLANES(gfx)                                     \
  (BG_PLANES((gfx)->blendTopBg) | ((gfx)->blendTopObj ? 16 : 0) | \
   ((gfx)->blendTopBackdrop ? 32 : 0))
#define BLEND_BOTTOM_PLANES(gfx)                                        \
  (BG_PLANES((gfx)->blendBottomBg) | ((gfx)->blendBottomObj ? 16 : 0) | \
   ((gfx)->blendBottomBackdrop ? 32 : 0))

#define WINDOW_PLANES(w) (BG_PLANES((w).bg) | ((w).obj ? 16 : 0))

/* Needed to match: these fields are read through a volatile view so
 * the compiler keeps their loads in source order around the register
 * accesses. */
#define VGFX ((volatile Unk_027c0280*)data_027c0280)

#define SET_BG_OFFSET(reg, i)                          \
  SetBgOffset(reg, VGFX->bgScrollX[i] - gScreenShakeX, \
              VGFX->bgScrollY[i] - gScreenShakeY)

#define SET_BG_CONTROL_TEXT(reg, i)                                        \
  SetBgControlText(reg, GFX->bgCnt[i].screenSize, GFX->bgCnt[i].colorMode, \
                   GFX->bgCnt[i].screenBase, GFX->bgCnt[i].charBase)

/* Needed to match: the empty condition emits no code, but gives the
 * affine terms the original's registers (found by a scripted search). */
#define SET_BG_CONTROL_AFFINE(reg, i)                                        \
  do {                                                                       \
    if ((GFX->bgCnt[i].screenBase | GFX->bgCnt[i].charBase |                 \
         GFX->bgCnt[i].extPltt) &&                                           \
        GFX->bgCnt[i].screenSize) {                                          \
    }                                                                        \
    SetBgControlAffine(reg, GFX->bgCnt[i].screenSize, GFX->bgCnt[i].extPltt, \
                       GFX->bgCnt[i].screenBase, GFX->bgCnt[i].charBase);    \
  } while (0)

/* Copies the main screen's state (data_027c0288.unk_00) to the main
 * engine's display registers, then restores the current screen. */
void Screen_ApplyMain(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);

  GX_SetGraphicsMode(GFX->dispMode, GFX_DISPCNT(GFX)->bgMode, GFX->bg0Mode);
  SetVisiblePlane(
      (GFX_DISPCNT(GFX)->bg0 ? 1 : 0) | (GFX_DISPCNT(GFX)->bg1 ? 2 : 0) |
      (GFX_DISPCNT(GFX)->bg2 ? 4 : 0) | (GFX_DISPCNT(GFX)->bg3 ? 8 : 0) |
      (GFX_DISPCNT(GFX)->obj ? 16 : 0));
  SetVisibleWnd((GFX_DISPCNT(GFX)->win0 ? 1 : 0) |
                (GFX_DISPCNT(GFX)->win1 ? 2 : 0) |
                (GFX_DISPCNT(GFX)->objWin ? 4 : 0));
  SetObjVramModeChar((GFX_DISPCNT(GFX)->objMap1D << 4) | (VGFX->unk_dd << 20));
  SetObjHBlankProc(GFX_DISPCNT(GFX)->objHBlankProc);
  if (GFX_DISPCNT(GFX)->displayOff) {
    GX_DispOff();
  } else {
    GX_DispOn();
  }
  GX_HBlankIntr(GFX->hBlankIrq ? TRUE : FALSE);
  GX_VBlankIntr(GFX->vBlankIrq ? TRUE : FALSE);
  SetVCountIrq(GFX->vCountIrq);
  func_0210a5d4(GFX->vCountMatch);

  if (GFX->bg0Mode != 1) {
    SET_BG_OFFSET(IO_BG0_SCROLL, 0);
  } else {
    func_0210b684(GFX->bgScrollX[0] - gScreenShakeX);
  }
  SET_BG_OFFSET(IO_BG1_SCROLL, 1);
  SET_BG_OFFSET(REG_BG2OFS, 2);
  SET_BG_OFFSET(IO_BG3_SCROLL, 3);
  SetBgScreenOffset(VGFX->bgScreenOffset);

  if (GFX->bg0Mode == 0) {
    SET_BG_CONTROL_TEXT(REG_BG0CNT, 0);
  }
  SET_BG_CONTROL_TEXT(REG_BG1CNT, 1);
  if (Screen_IsTextBg(2) == TRUE) {
    SET_BG_CONTROL_TEXT(REG_BG2CNT, 2);
  } else {
    SET_BG_CONTROL_AFFINE(REG_BG2CNT, 2);
    *REG_BG2PA = GFX->bg2Affine.pa;
    *REG_BG2PB = GFX->bg2Affine.pb;
    *REG_BG2PC = GFX->bg2Affine.pc;
    *REG_BG2PD = GFX->bg2Affine.pd;
    *REG_BG2X = GFX->bg2Affine.x;
    *REG_BG2Y = GFX->bg2Affine.y;
  }
  if (Screen_IsTextBg(3) == TRUE) {
    SET_BG_CONTROL_TEXT(REG_BG3CNT, 3);
  } else {
    SET_BG_CONTROL_AFFINE(REG_BG3CNT, 3);
    *REG_BG3PA = GFX->bg3Affine.pa;
    *REG_BG3PB = GFX->bg3Affine.pb;
    *REG_BG3PC = GFX->bg3Affine.pc;
    *REG_BG3PD = GFX->bg3Affine.pd;
    *REG_BG3X = GFX->bg3Affine.x;
    *REG_BG3Y = GFX->bg3Affine.y;
  }

  SetBgPriority(REG_BG0CNT, GFX->bgCnt[0].priority);
  SetBgPriority(REG_BG1CNT, GFX->bgCnt[1].priority);
  SetBgPriority(REG_BG2CNT, GFX->bgCnt[2].priority);
  SetBgPriority(REG_BG3CNT, GFX->bgCnt[3].priority);
  SetBgMosaic(REG_BG0CNT, GFX->bgCnt[0].mosaic);
  SetBgMosaic(REG_BG1CNT, GFX->bgCnt[1].mosaic);
  SetBgMosaic(REG_BG2CNT, GFX->bgCnt[2].mosaic);
  SetBgMosaic(REG_BG3CNT, GFX->bgCnt[3].mosaic);
  SetMosaicSize(IO_BG_PIXELATE_SIZE, GFX->bgMosaicH, GFX->bgMosaicV);
  SetMosaicSize(IO_OBJ_PIXELATE_SIZE, GFX->objMosaicH, GFX->objMosaicV);

  switch (GFX->blendMode) {
    case 0:
    default:
      *REG_BLDCNT = 0;
      break;
    case 1:
      SetBlendAlpha(BLEND_TOP_PLANES(GFX), BLEND_BOTTOM_PLANES(GFX),
                    VGFX->blendEva, VGFX->blendEvb);
      break;
    case 2:
      SetBlendBrightness(BLEND_TOP_PLANES(GFX), BLEND_BOTTOM_PLANES(GFX),
                         VGFX->blendEva, VGFX->blendEvb, VGFX->blendEvy);
      break;
    case 3:
      SetBlendBrightness(BLEND_TOP_PLANES(GFX), BLEND_BOTTOM_PLANES(GFX),
                         VGFX->blendEva, VGFX->blendEvb, -VGFX->blendEvy);
      break;
  }

  SetWnd0Position(VGFX->win0Left, VGFX->win0Top, VGFX->win0Right,
                  VGFX->win0Bottom);
  SetWnd1Position(VGFX->win1Left, VGFX->win1Top, VGFX->win1Right,
                  VGFX->win1Bottom);
  SetWndPlaneLow(REG_WININ, WINDOW_PLANES(GFX->win0In),
                 GFX->win0In.effect ? TRUE : FALSE);
  SetWndPlaneHigh(REG_WININ, WINDOW_PLANES(GFX->win1In),
                  GFX->win1In.effect ? TRUE : FALSE);
  SetWndPlaneLow(REG_WINOUT, WINDOW_PLANES(GFX->winOut),
                 GFX->winOut.effect ? TRUE : FALSE);
  SetWndPlaneHigh(REG_WINOUT, WINDOW_PLANES(GFX->objWinIn),
                  GFX->objWinIn.effect ? TRUE : FALSE);
  GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, GFX->masterBrightness);

  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
