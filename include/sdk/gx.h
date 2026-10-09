#ifndef SDK_GX_H
#define SDK_GX_H

#include "types.h"

#define REG_MASTER_BRIGHT ((volatile u16 *)0x0400006c)
#define REG_BG2CNT ((volatile u16 *)0x0400000c)
/* Main engine display registers (GBATEK). */
#define REG_DISPCNT ((volatile u32 *)0x04000000)
#define REG_DISPSTAT ((volatile u16 *)0x04000004)
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
#define POWCNT1_DISPLAY_SWAP 0x8000 /* set: main engine on the top screen */

/* What GX_SetMasterBrightness calls: writes a master brightness register.
 * brightness: 0 none, 1-16 towards white, -1 to -16 towards black. */
void GXx_SetMasterBrightness_(volatile u16 *reg, int brightness);

#endif
