#include "types.h"
#include "game/screen_shake.h"
#include "sdk/gx.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

BOOL Screen_IsTextBg(int bg);
void GXS_SetGraphicsMode(int bgMode);
void G2x_SetBlendBrightnessExt(volatile u16* reg, int top, int bottom, int eva,
                               int evb, int evy);
void G2x_SetBlendAlpha(volatile u16* reg, int top, int bottom, int eva,
                       int evb);

#define GFX data_027c0280

/* The engine B versions of the inline helpers in screen_apply_main.c,
 * writing the engine B registers (DB_DISPCNT and the 0x04001000 block). */

static inline void SetVisiblePlane(int planes) {
  *REG_DB_DISPCNT = (*REG_DB_DISPCNT & ~0x1f00) | (planes << 8);
}

static inline void SetVisibleWnd(int windows) {
  *REG_DB_DISPCNT = (*REG_DB_DISPCNT & ~0xe000) | (windows << 13);
}

static inline void SetObjVramModeChar(int mode) {
  *REG_DB_DISPCNT = (*REG_DB_DISPCNT & ~0x300010) | mode;
}

static inline void SetObjHBlankProc(BOOL on) {
  if (on) {
    *REG_DB_DISPCNT |= 0x800000;
  } else {
    *REG_DB_DISPCNT &= ~0x800000;
  }
}

static inline void SetBgOffset(volatile u32* reg, int x, int y) {
  *reg = (x & 0x1ff) | ((y << 16) & 0x1ff0000);
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
  G2x_SetBlendAlpha(REG_DB_BLDCNT, top, bottom, eva, evb);
}

static inline void SetBlendBrightness(int top, int bottom, int eva, int evb,
                                      int evy) {
  G2x_SetBlendBrightnessExt(REG_DB_BLDCNT, top, bottom, eva, evb, evy);
}

static inline void SetWnd0Position(int left, int top, int right, int bottom) {
  *REG_DB_WIN0H = ((left << 8) & 0xff00) | (right & 0xff);
  *REG_DB_WIN0V = ((top << 8) & 0xff00) | (bottom & 0xff);
}

static inline void SetWnd1Position(int left, int top, int right, int bottom) {
  *REG_DB_WIN1H = ((left << 8) & 0xff00) | (right & 0xff);
  *REG_DB_WIN1V = ((top << 8) & 0xff00) | (bottom & 0xff);
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

/* Copies the sub screen's state (data_027c0288.unk_04) to the sub
 * engine's display registers, then restores the current screen. */
void Screen_ApplySub(void) {
  Unk_027c0280* savedGfx;
  Unk_02080440* savedState;

  savedGfx = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  savedState = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);

  GXS_SetGraphicsMode(GFX_DISPCNT(GFX)->bgMode);
  SetVisiblePlane(
      (GFX_DISPCNT(GFX)->bg0 ? 1 : 0) | (GFX_DISPCNT(GFX)->bg1 ? 2 : 0) |
      (GFX_DISPCNT(GFX)->bg2 ? 4 : 0) | (GFX_DISPCNT(GFX)->bg3 ? 8 : 0) |
      (GFX_DISPCNT(GFX)->obj ? 16 : 0));
  SetVisibleWnd((GFX_DISPCNT(GFX)->win0 ? 1 : 0) |
                (GFX_DISPCNT(GFX)->win1 ? 2 : 0) |
                (GFX_DISPCNT(GFX)->objWin ? 4 : 0));
  SetObjVramModeChar((GFX_DISPCNT(GFX)->objMap1D << 4) | (VGFX->unk_dd << 20));
  SetObjHBlankProc(GFX_DISPCNT(GFX)->objHBlankProc);
  /* DISPCNT bit 16 is the sub engine's display mode: 0 off, 1 on. */
  if (GFX_DISPCNT(GFX)->displayOff) {
    *REG_DB_DISPCNT &= ~0x10000;
  } else {
    *REG_DB_DISPCNT |= 0x10000;
  }

  SET_BG_OFFSET(IO_SUB_BG0_SCROLL, 0);
  SET_BG_OFFSET(REG_DB_BG1OFS, 1);
  SET_BG_OFFSET(REG_DB_BG2OFS, 2);
  SET_BG_OFFSET(REG_DB_BG3OFS, 3);

  SET_BG_CONTROL_TEXT(REG_DB_BG0CNT, 0);
  SET_BG_CONTROL_TEXT(REG_DB_BG1CNT, 1);
  if (Screen_IsTextBg(2) == TRUE) {
    SET_BG_CONTROL_TEXT(REG_DB_BG2CNT, 2);
  } else {
    SET_BG_CONTROL_AFFINE(REG_DB_BG2CNT, 2);
    *REG_DB_BG2PA = GFX->bg2Affine.pa;
    *REG_DB_BG2PB = GFX->bg2Affine.pb;
    *REG_DB_BG2PC = GFX->bg2Affine.pc;
    *REG_DB_BG2PD = GFX->bg2Affine.pd;
    *REG_DB_BG2X = GFX->bg2Affine.x;
    *REG_DB_BG2Y = GFX->bg2Affine.y;
  }
  if (Screen_IsTextBg(3) == TRUE) {
    SET_BG_CONTROL_TEXT(REG_DB_BG3CNT, 3);
  } else {
    SET_BG_CONTROL_AFFINE(REG_DB_BG3CNT, 3);
    *REG_DB_BG3PA = GFX->bg3Affine.pa;
    *REG_DB_BG3PB = GFX->bg3Affine.pb;
    *REG_DB_BG3PC = GFX->bg3Affine.pc;
    *REG_DB_BG3PD = GFX->bg3Affine.pd;
    *REG_DB_BG3X = GFX->bg3Affine.x;
    *REG_DB_BG3Y = GFX->bg3Affine.y;
  }

  SetBgPriority(REG_DB_BG0CNT, GFX->bgCnt[0].priority);
  SetBgPriority(REG_DB_BG1CNT, GFX->bgCnt[1].priority);
  SetBgPriority(REG_DB_BG2CNT, GFX->bgCnt[2].priority);
  SetBgPriority(REG_DB_BG3CNT, GFX->bgCnt[3].priority);
  SetBgMosaic(REG_DB_BG0CNT, GFX->bgCnt[0].mosaic);
  SetBgMosaic(REG_DB_BG1CNT, GFX->bgCnt[1].mosaic);
  SetBgMosaic(REG_DB_BG2CNT, GFX->bgCnt[2].mosaic);
  SetBgMosaic(REG_DB_BG3CNT, GFX->bgCnt[3].mosaic);
  SetMosaicSize(IO_SUB_BG_PIXELATE_SIZE, GFX->bgMosaicH, GFX->bgMosaicV);
  SetMosaicSize(IO_SUB_OBJ_PIXELATE_SIZE, GFX->objMosaicH, GFX->objMosaicV);

  switch (GFX->blendMode) {
    case 0:
    default:
      *REG_DB_BLDCNT = 0;
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
  SetWndPlaneLow(REG_DB_WININ, WINDOW_PLANES(GFX->win0In),
                 GFX->win0In.effect ? TRUE : FALSE);
  SetWndPlaneHigh(REG_DB_WININ, WINDOW_PLANES(GFX->win1In),
                  GFX->win1In.effect ? TRUE : FALSE);
  SetWndPlaneLow(REG_DB_WINOUT, WINDOW_PLANES(GFX->winOut),
                 GFX->winOut.effect ? TRUE : FALSE);
  SetWndPlaneHigh(REG_DB_WINOUT, WINDOW_PLANES(GFX->objWinIn),
                  GFX->objWinIn.effect ? TRUE : FALSE);
  GXx_SetMasterBrightness_(REG_DB_MASTER_BRIGHT, GFX->masterBrightness);

  Screen_SetCurrent(savedState);
  Screen_SetCurrentGfx(savedGfx);
}
