#ifndef SDK_GX_H
#define SDK_GX_H

#include "types.h"

#define REG_MASTER_BRIGHT ((volatile u16 *)0x0400006c)
#define IO_SUB_SCREEN_FADE ((volatile u16 *)0x0400106c)
#define REG_BG2CNT ((volatile u16 *)0x0400000c)
/* Main engine display registers (GBATEK). */
#define REG_DISPCNT ((volatile u32 *)0x04000000)
#define REG_DISPSTAT ((volatile u16 *)0x04000004)
#define REG_VCOUNT ((volatile u16 *)0x04000006)
#define HW_BG_VRAM 0x06000000 /* main engine BG VRAM */
#define REG_BG0CNT ((volatile u16 *)0x04000008)
#define REG_BG1CNT ((volatile u16 *)0x0400000a)
#define REG_BG3CNT ((volatile u16 *)0x0400000e)
#define IO_BG0_SCROLL ((volatile u32 *)0x04000010)
#define IO_BG1_SCROLL ((volatile u32 *)0x04000014)
#define REG_BG2OFS ((volatile u32 *)0x04000018)
#define IO_BG3_SCROLL ((volatile u32 *)0x0400001c)
#define REG_BG2PA ((volatile s16 *)0x04000020)
#define REG_BG2PB ((volatile s16 *)0x04000022)
#define REG_BG2PC ((volatile s16 *)0x04000024)
#define REG_BG2PD ((volatile s16 *)0x04000026)
#define REG_BG2X ((volatile s32 *)0x04000028)
#define REG_BG2Y ((volatile s32 *)0x0400002c)
#define REG_BG3PA ((volatile s16 *)0x04000030)
#define REG_BG3PB ((volatile s16 *)0x04000032)
#define REG_BG3PC ((volatile s16 *)0x04000034)
#define REG_BG3PD ((volatile s16 *)0x04000036)
#define REG_BG3X ((volatile s32 *)0x04000038)
#define REG_BG3Y ((volatile s32 *)0x0400003c)
#define REG_WIN0H ((volatile u16 *)0x04000040)
#define REG_WIN1H ((volatile u16 *)0x04000042)
#define REG_WIN0V ((volatile u16 *)0x04000044)
#define REG_WIN1V ((volatile u16 *)0x04000046)
#define REG_WININ ((volatile u16 *)0x04000048)
#define REG_WINOUT ((volatile u16 *)0x0400004a)
#define IO_BG_PIXELATE_SIZE ((volatile u8 *)0x0400004c)
#define IO_OBJ_PIXELATE_SIZE ((volatile u8 *)0x0400004d)
#define REG_BLDCNT ((volatile u16 *)0x04000050)
#define REG_POWCNT1 ((volatile u16 *)0x04000304)
#define REG_MTX_STORE ((volatile u32 *)0x0400044c)
#define POWCNT1_DISPLAY_SWAP 0x8000 /* set: main engine on the top screen */

/* Sub engine (2D graphics engine B) registers. */
#define REG_DB_DISPCNT ((volatile u32 *)0x04001000)
#define REG_DB_BG0CNT ((volatile u16 *)0x04001008)
#define REG_DB_BG1CNT ((volatile u16 *)0x0400100a)
#define REG_DB_BG2CNT ((volatile u16 *)0x0400100c)
#define REG_DB_BG3CNT ((volatile u16 *)0x0400100e)
#define IO_SUB_BG0_SCROLL ((volatile u32 *)0x04001010)
#define REG_DB_BG0HOFS ((volatile u16 *)0x04001010)
#define REG_DB_BG0VOFS ((volatile u16 *)0x04001012)
#define REG_DB_BG1OFS ((volatile u32 *)0x04001014)
#define REG_DB_BG2OFS ((volatile u32 *)0x04001018)
#define REG_DB_BG3OFS ((volatile u32 *)0x0400101c)
#define REG_DB_BG2PA ((volatile s16 *)0x04001020)
#define REG_DB_BG2PB ((volatile s16 *)0x04001022)
#define REG_DB_BG2PC ((volatile s16 *)0x04001024)
#define REG_DB_BG2PD ((volatile s16 *)0x04001026)
#define REG_DB_BG2X ((volatile s32 *)0x04001028)
#define REG_DB_BG2Y ((volatile s32 *)0x0400102c)
#define REG_DB_BG3PA ((volatile s16 *)0x04001030)
#define REG_DB_BG3PB ((volatile s16 *)0x04001032)
#define REG_DB_BG3PC ((volatile s16 *)0x04001034)
#define REG_DB_BG3PD ((volatile s16 *)0x04001036)
#define REG_DB_BG3X ((volatile s32 *)0x04001038)
#define REG_DB_BG3Y ((volatile s32 *)0x0400103c)
#define REG_DB_WIN0H ((volatile u16 *)0x04001040)
#define REG_DB_WIN1H ((volatile u16 *)0x04001042)
#define REG_DB_WIN0V ((volatile u16 *)0x04001044)
#define REG_DB_WIN1V ((volatile u16 *)0x04001046)
#define REG_DB_WININ ((volatile u16 *)0x04001048)
#define REG_DB_WINOUT ((volatile u16 *)0x0400104a)
#define IO_SUB_BG_PIXELATE_SIZE ((volatile u8 *)0x0400104c)
#define IO_SUB_OBJ_PIXELATE_SIZE ((volatile u8 *)0x0400104d)
#define REG_DB_BLDCNT ((volatile u16 *)0x04001050)
#define REG_DB_BLDALPHA ((volatile u16 *)0x04001052)
#define REG_DB_MASTER_BRIGHT ((volatile u16 *)0x0400106c)

/* VRAM bank control (one byte per bank; A-D are written together as a word). */
#define REG_VRAMCNT_ABCD ((volatile u32 *)0x04000240)
#define REG_VRAMCNT_A ((volatile u8 *)0x04000240)
#define REG_VRAMCNT_B ((volatile u8 *)0x04000241)
#define REG_VRAMCNT_C ((volatile u8 *)0x04000242)
#define REG_VRAMCNT_D ((volatile u8 *)0x04000243)
#define REG_VRAMCNT_E ((volatile u8 *)0x04000244)
#define REG_VRAMCNT_F ((volatile u8 *)0x04000245)
#define REG_VRAMCNT_G ((volatile u8 *)0x04000246)
#define REG_VRAMCNT_H ((volatile u8 *)0x04000248)
#define REG_VRAMCNT_I ((volatile u8 *)0x04000249)
#define IO_VRAM_BANKS_H_I_CTRL ((volatile u16 *)0x04000248)

/* 3D engine registers. */
#define REG_DISP3DCNT ((volatile u16 *)0x04000060)
#define REG_MTX_MODE ((volatile u32 *)0x04000440)
#define REG_MTX_PUSH ((volatile u32 *)0x04000444)
#define REG_MTX_POP ((volatile u32 *)0x04000448)
#define REG_MTX_STORE ((volatile u32 *)0x0400044c)
#define REG_MTX_IDENTITY ((volatile u32 *)0x04000454)
#define REG_MTX_TRANS ((volatile u32 *)0x04000470)
#define REG_POLYGON_ATTR ((volatile u32 *)0x040004a4)
#define REG_TEXIMAGE_PARAM ((volatile u32 *)0x040004a8)
#define REG_PLTT_BASE ((volatile u32 *)0x040004ac)
#define REG_BEGIN_VTXS ((volatile u32 *)0x04000500)
#define REG_END_VTXS ((volatile u32 *)0x04000504)
#define REG_SWAP_BUFFERS ((volatile u32 *)0x04000540)
#define REG_VIEWPORT ((volatile u32 *)0x04000580)

/* What GX_SetMasterBrightness calls: writes a master brightness register.
 * brightness: 0 none, 1-16 towards white, -1 to -16 towards black. */
void GXx_SetMasterBrightness_(volatile u16 *reg, int brightness);

/* Display state kept by GX_SetGraphicsMode, GX_DispOn and GX_DispOff.
 * gGxDispMode: display mode last requested (DISPCNT bits 16-17).
 * gGxDispOn: 0 while the display is switched off (starts at 1). */
extern u16 gGxDispMode;
extern u16 gGxDispOn;

void GX_SetGraphicsMode(int dispMode, int bgMode, int bg0As3d);
void GX_DispOn(void);
void GX_DispOff(void);

#endif
