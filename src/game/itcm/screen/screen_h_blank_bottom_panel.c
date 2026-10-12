#include "types.h"
#include "sdk/gx.h"
#include "game/screen_hblank.h"

extern u8* data_02297790; /* layer 0 tile data in main BG VRAM */

int func_0201d834(int priority);     /* sets the BG1CNT priority */
int func_0201d8c0(int screenOffset); /* GX_SetBGScrOffset */
int func_0201d8d8(int windows);      /* GX_SetVisibleWnd */
int func_0201d8f0(int planes);       /* GX_SetVisiblePlane */
void Bg1_SetControl(int screenSize, int colorMode, int screenBase, int charBase,
                    int extPltt);
void Bg2_SetControl(int screenSize, int colorMode, int screenBase,
                    int charBase);
void Bg2_SetPriority(int priority);
BOOL Screen_IsTextBg(int layer);

/* H-blank handler that shows a panel at the bottom of the main screen, from
 * scanline 144 + data_02297788 down: BG1 and BG2 only, with screen bases 30
 * and 31, no windows or blending. Two lines early it turns BG1 and BG2 on.
 * On lines 261 and 262, before the next frame, it puts back the registers
 * Screen_PrepareHBlankRegs prepared. */
void Screen_HBlankBottomPanel(void) {
  int line = *REG_VCOUNT;

  if (line == data_02297788 + 141 || line == data_02297788 + 142) {
    HBlank_SetVisiblePlane(data_027c0270 | 6);
  }
  if (line >= data_02297788 + 143 && line < 192) {
    func_0201d8f0(6);
    func_0201d8d8(0);
    *IO_BG1_SCROLL = ((112 - data_02297788) << 16) & 0x1ff0000;
    *REG_BG2OFS = ((112 - data_02297788) << 16) & 0x1ff0000;
    *(volatile u32*)REG_BLDCNT = 0;
    func_0201d8c0(1);
    Bg1_SetControl(0, 0, 30, ((u32)data_02297790 - HW_BG_VRAM) >> 14, 0);
    Bg2_SetControl(0, 0, 31, ((u32)data_02297790 - HW_BG_VRAM) >> 14);
    func_0201d834(0);
    Bg2_SetPriority(0);
  }
  if (line == 261 || line == 262) {
    HBlank_SetVisiblePlane(data_027c0270);
    HBlank_SetVisibleWnd(data_027c026c);
    *IO_BG1_SCROLL = data_027c0264;
    *REG_BG2OFS = data_027c0274;
    *(volatile u32*)REG_BLDCNT = data_027c0268;
    HBlank_SetBgScreenOffset(data_027c0238);
    *REG_BG1CNT = data_027c0254;
    *REG_BG2CNT = data_027c0258;
    if (Screen_IsTextBg(2)) {
      return;
    }
    *REG_BG2PA = data_027c025c;
    *REG_BG2PB = data_027c0260;
    *REG_BG2PC = data_027c023c;
    *REG_BG2PD = data_027c0248;
    *REG_BG2X = data_027c0250;
    *REG_BG2Y = data_027c0244;
  }
}
