#ifndef SDK_GX_H
#define SDK_GX_H

#include "types.h"
#include "sdk/fx.h"

#define REG_MASTER_BRIGHT ((volatile u16*)0x0400006c)
#define IO_SUB_SCREEN_FADE ((volatile u16*)0x0400106c)
#define REG_BG2CNT ((volatile u16*)0x0400000c)
/* Main engine display registers (GBATEK). */
#define REG_DISPCNT ((volatile u32*)0x04000000)
#define DISPCNT_BG_EXT_PLTT 0x40000000 /* BG extended palettes on */
#define REG_DISPSTAT ((volatile u16*)0x04000004)
#define REG_VCOUNT ((volatile u16*)0x04000006)
#define HW_BG_VRAM 0x06000000 /* main engine BG VRAM */
#define REG_BG0CNT ((volatile u16*)0x04000008)
#define REG_BG1CNT ((volatile u16*)0x0400000a)
#define REG_BG3CNT ((volatile u16*)0x0400000e)
#define IO_BG0_SCROLL ((volatile u32*)0x04000010)
#define IO_BG1_SCROLL ((volatile u32*)0x04000014)
#define REG_BG2OFS ((volatile u32*)0x04000018)
#define IO_BG3_SCROLL ((volatile u32*)0x0400001c)
#define REG_BG2PA ((volatile s16*)0x04000020)
#define REG_BG2PB ((volatile s16*)0x04000022)
#define REG_BG2PC ((volatile s16*)0x04000024)
#define REG_BG2PD ((volatile s16*)0x04000026)
#define REG_BG2X ((volatile s32*)0x04000028)
#define REG_BG2Y ((volatile s32*)0x0400002c)
#define REG_BG3PA ((volatile s16*)0x04000030)
#define REG_BG3PB ((volatile s16*)0x04000032)
#define REG_BG3PC ((volatile s16*)0x04000034)
#define REG_BG3PD ((volatile s16*)0x04000036)
#define REG_BG3X ((volatile s32*)0x04000038)
#define REG_BG3Y ((volatile s32*)0x0400003c)
#define REG_WIN0H ((volatile u16*)0x04000040)
#define REG_WIN1H ((volatile u16*)0x04000042)
#define REG_WIN0V ((volatile u16*)0x04000044)
#define REG_WIN1V ((volatile u16*)0x04000046)
#define REG_WININ ((volatile u16*)0x04000048)
#define REG_WINOUT ((volatile u16*)0x0400004a)
#define IO_BG_PIXELATE_SIZE ((volatile u8*)0x0400004c)
#define IO_OBJ_PIXELATE_SIZE ((volatile u8*)0x0400004d)
#define REG_BLDCNT ((volatile u16*)0x04000050)
#define REG_POWCNT1 ((volatile u16*)0x04000304)
#define REG_MTX_STORE ((volatile u32*)0x0400044c)
#define POWCNT1_DISPLAY_SWAP 0x8000 /* set: main engine on the top screen */
#define POWCNT1_LCD 0x1
#define POWCNT1_ENGINES \
  0x20e /* 2D engine A, 3D rendering, 3D geometry, 2D engine B */

/* Sub engine (2D graphics engine B) registers. */
#define REG_DB_DISPCNT ((volatile u32*)0x04001000)
#define REG_DB_BG0CNT ((volatile u16*)0x04001008)
#define REG_DB_BG1CNT ((volatile u16*)0x0400100a)
#define REG_DB_BG2CNT ((volatile u16*)0x0400100c)
#define REG_DB_BG3CNT ((volatile u16*)0x0400100e)
#define IO_SUB_BG0_SCROLL ((volatile u32*)0x04001010)
#define REG_DB_BG0HOFS ((volatile u16*)0x04001010)
#define REG_DB_BG0VOFS ((volatile u16*)0x04001012)
#define REG_DB_BG1OFS ((volatile u32*)0x04001014)
#define REG_DB_BG2OFS ((volatile u32*)0x04001018)
#define REG_DB_BG3OFS ((volatile u32*)0x0400101c)
#define REG_DB_BG2PA ((volatile s16*)0x04001020)
#define REG_DB_BG2PB ((volatile s16*)0x04001022)
#define REG_DB_BG2PC ((volatile s16*)0x04001024)
#define REG_DB_BG2PD ((volatile s16*)0x04001026)
#define REG_DB_BG2X ((volatile s32*)0x04001028)
#define REG_DB_BG2Y ((volatile s32*)0x0400102c)
#define REG_DB_BG3PA ((volatile s16*)0x04001030)
#define REG_DB_BG3PB ((volatile s16*)0x04001032)
#define REG_DB_BG3PC ((volatile s16*)0x04001034)
#define REG_DB_BG3PD ((volatile s16*)0x04001036)
#define REG_DB_BG3X ((volatile s32*)0x04001038)
#define REG_DB_BG3Y ((volatile s32*)0x0400103c)
#define REG_DB_WIN0H ((volatile u16*)0x04001040)
#define REG_DB_WIN1H ((volatile u16*)0x04001042)
#define REG_DB_WIN0V ((volatile u16*)0x04001044)
#define REG_DB_WIN1V ((volatile u16*)0x04001046)
#define REG_DB_WININ ((volatile u16*)0x04001048)
#define REG_DB_WINOUT ((volatile u16*)0x0400104a)
#define IO_SUB_BG_PIXELATE_SIZE ((volatile u8*)0x0400104c)
#define IO_SUB_OBJ_PIXELATE_SIZE ((volatile u8*)0x0400104d)
#define REG_DB_BLDCNT ((volatile u16*)0x04001050)
#define REG_DB_BLDALPHA ((volatile u16*)0x04001052)
#define REG_DB_MASTER_BRIGHT ((volatile u16*)0x0400106c)

/* VRAM bank control (one byte per bank; A-D are written together as a word). */
#define REG_VRAMCNT_ABCD ((volatile u32*)0x04000240)
#define REG_VRAMCNT_A ((volatile u8*)0x04000240)
#define REG_VRAMCNT_B ((volatile u8*)0x04000241)
#define REG_VRAMCNT_C ((volatile u8*)0x04000242)
#define REG_VRAMCNT_D ((volatile u8*)0x04000243)
#define REG_VRAMCNT_E ((volatile u8*)0x04000244)
#define REG_VRAMCNT_F ((volatile u8*)0x04000245)
#define REG_VRAMCNT_G ((volatile u8*)0x04000246)
#define REG_VRAMCNT_H ((volatile u8*)0x04000248)
#define REG_VRAMCNT_I ((volatile u8*)0x04000249)
#define VRAMCNT_ENABLE 0x80
#define VRAMCNT_TEXPLTT(slot) \
  (0x83 | ((slot) << 3)) /* enabled, texture palette slot */
#define VRAMCNT_TEX(slot) \
  (0x83 | ((slot) << 3)) /* banks A-D: enabled, texture image slot */
#define VRAMCNT_BG(offset) \
  (0x81 |                  \
   ((offset)               \
    << 3)) /* enabled, main engine BG; what the offset field means depends on the bank (GBATEK) */
#define VRAMCNT_OBJ(offset) \
  (0x82 |                   \
   ((offset)                \
    << 3)) /* enabled, main engine OBJ; what the offset field means depends on the bank (GBATEK) */
#define VRAMCNT_BGEXTPLTT(slot) \
  (0x84 | ((slot) << 3)) /* banks E-G: enabled, main BG extended palette slot */
#define VRAMCNT_SUB_BGEXTPLTT \
  0x82 /* bank H: enabled, sub engine BG extended palettes */
#define GX_VRAM_A 0x001
#define GX_VRAM_B 0x002
#define GX_VRAM_C 0x004
#define GX_VRAM_D 0x008
#define GX_VRAM_E 0x010
#define GX_VRAM_F 0x020
#define GX_VRAM_G 0x040
#define GX_VRAM_H 0x080
#define GX_VRAM_I 0x100

/* Geometry engine status register. */
#define REG_GXSTAT ((volatile u32*)0x04000600)
#define GXSTAT_PV_LEVEL 0x1f00            /* position/vector stack level */
#define GXSTAT_PJ_LEVEL 0x2000            /* projection stack level */
#define GXSTAT_MTX_STACK_ERROR 0x4000     /* stack overflow or underflow */
#define GXSTAT_MTX_STACK_ERROR_ACK 0x8000 /* written 1 to clear the error */
#define GXSTAT_GEOMETRY_BUSY 0x08000000
#define GXSTAT_FIFO_IRQ_MASK 0xc0000000
#define GXSTAT_FIFO_IRQ_EMPTY \
  0x80000000 /* command FIFO interrupt when the FIFO is empty */
#define REG_GXFIFO ((volatile u32*)0x04000400)
/* Command bytes written to REG_GXFIFO, each followed by its parameter words. */
#define GXFIFO_CMD_MTX_LOAD_4X4 0x16
#define GXFIFO_CMD_MTX_MULT_4X4 0x18
#define GXFIFO_CMD_MTX_MULT_3X3 0x1a
#define REG_CLEAR_COLOR ((volatile u32*)0x04000350)
#define REG_CLEAR_DEPTH ((volatile u16*)0x04000354)
#define IO_3D_REAR_PLANE_SCROLL ((volatile u16*)0x04000356)
#define REG_FOG_COLOR ((volatile u32*)0x04000358)
#define REG_FOG_OFFSET ((volatile u16*)0x0400035c)
#define CLEAR_COLOR_FOG 0x8000
#define REG_EDGE_COLOR ((volatile u16*)0x04000330)
#define REG_FOG_TABLE \
  ((volatile u8*)0x04000360) /* fog table (32 bytes), then toon table (64 bytes) */
#define REG_SHININESS ((volatile u32*)0x040004d0)
/* Sizes of the 3D tables above, in bytes (GBATEK). */
#define EDGE_COLOR_TABLE_SIZE 16 /* 8 colors */
#define FOG_TABLE_SIZE 32
#define TOON_TABLE_SIZE 64
#define SHININESS_TABLE_WORDS 32 /* 128 bytes, written a word at a time */
/* Display registers G2/GX initialisation zeroes: the main engine's from
   BG0CNT up to 0x04000068, the sub engine's from DISPCNT through
   MASTER_BRIGHT. */
#define MAIN_DISPLAY_REGS_CLEAR_SIZE 0x60
#define SUB_DISPLAY_REGS_CLEAR_SIZE 0x70
#define REG_POLYGON_ATTR ((volatile u32*)0x040004a4)
#define REG_TEXIMAGE_PARAM ((volatile u32*)0x040004a8)
#define REG_PLTT_BASE ((volatile u32*)0x040004ac)
#define DISP3DCNT_TEXTURE 0x0001 /* texture mapping on */
#define DISP3DCNT_HIGHLIGHT_SHADING \
  0x0002 /* set: highlight shading, clear: toon shading */
#define DISP3DCNT_ALPHA_TEST 0x0004
#define DISP3DCNT_ANTI_ALIAS 0x0010
#define DISP3DCNT_UNDERFLOW_ACK \
  0x2000 /* write 1 to clear the line buffer underflow flag */
#define DISP3DCNT_OVERFLOW_ACK \
  0x1000 /* write 1 to clear the polygon/vertex RAM overflow flag */

/* MTX_MODE values. */
#define GX_MTXMODE_PROJECTION 0
#define GX_MTXMODE_POSITION_VECTOR 2
#define GX_MTXMODE_TEXTURE 3

/* BLDCNT color effect field (bits 6-7). */
#define BLDCNT_EFFECT_ALPHA 0x40
#define BLDCNT_EFFECT_BRIGHTEN 0x80
#define BLDCNT_EFFECT_DARKEN 0xc0

void Vram_PowerOnBanks(u32 banks);
void GX_SetBankForBG(int bg);
void GX_SetBankForTex(int tex);
void GX_SetBankForBGExtPltt(int bgExtPltt);
void GX_SetBankForOBJ(int obj);
void GX_SetBankForTexPltt(int texPltt);
void GXi_NopClearFifo128_(volatile u32* fifo);
int G3X_GetMtxStackLevelPJ(int* level);
int G3X_GetMtxStackLevelPV(int* level);
void G3X_ResetMtxStack(void);
void G3X_InitMtxStack(void);
void G3X_ClearFifo(void);
void G3X_Reset(void);
void G3X_Init(void);
void G3X_InitTable(void);
void G3X_SetClearColor(int color, int alpha, int depth, int polygonId,
                       BOOL fog);
#define IO_VRAM_BANKS_H_I_CTRL ((volatile u16*)0x04000248)

/* 3D engine registers. */
#define REG_DISP3DCNT ((volatile u16*)0x04000060)
#define REG_MTX_MODE ((volatile u32*)0x04000440)
#define REG_MTX_PUSH ((volatile u32*)0x04000444)
#define REG_MTX_POP ((volatile u32*)0x04000448)
#define REG_MTX_STORE ((volatile u32*)0x0400044c)
#define IO_GEOM_APPLY_MATRIX_3X3 ((volatile s32*)0x04000468)
#define REG_MTX_IDENTITY ((volatile u32*)0x04000454)
#define IO_GEOM_SET_MATRIX_4X4 ((volatile s32*)0x04000458)
#define IO_GEOM_SET_MATRIX_4X3 ((volatile s32*)0x0400045c)
#define REG_MTX_TRANS ((volatile u32*)0x04000470)
#define REG_POLYGON_ATTR ((volatile u32*)0x040004a4)
#define REG_TEXIMAGE_PARAM ((volatile u32*)0x040004a8)
#define REG_PLTT_BASE ((volatile u32*)0x040004ac)
#define REG_BEGIN_VTXS ((volatile u32*)0x04000500)
#define REG_END_VTXS ((volatile u32*)0x04000504)
#define REG_SWAP_BUFFERS ((volatile u32*)0x04000540)
#define REG_VIEWPORT ((volatile u32*)0x04000580)

/* What GX_SetMasterBrightness calls: writes a master brightness register.
 * brightness: 0 none, 1-16 towards white, -1 to -16 towards black. */
void GXx_SetMasterBrightness_(volatile u16* reg, int brightness);

/* Display state kept by GX_SetGraphicsMode, GX_DispOn and GX_DispOff.
 * gGxDispMode: display mode last requested (DISPCNT bits 16-17).
 * gGxDispOn: 0 while the display is switched off (starts at 1). */
extern u16 gGxDispMode;
extern u16 gGxDispOn;

void GX_SetGraphicsMode(int dispMode, int bgMode, int bg0As3d);
void GX_DispOn(void);
void GX_DispOff(void);
void GX_Init(void);
void Vram_ClearAllBankControl(void);

/* VRAM bank masks: [0] the banks given to the LCDC, then the banks of
 * each use the GX_SetBankFor* functions set (13 in all). */
extern u16 gGxVramBankMasks[];
/* Lock id GX_Init takes for the VRAM banks. */
extern u16 GXi_VRamLockId;
#define GX_DMA_NOT_USE \
  ((u32) - 1) /* GX library DMA channel setting: copy with the CPU */

/* Load mtx into the current matrix, or multiply the current matrix by it. */
void Geom_LoadMatrix44(const MtxFx44* mtx);
void Geom_MultMatrix44(const MtxFx44* mtx);
void Geom_MultMatrix33(const MtxFx33* mtx);

/* Multiplies the current matrix by a rotation about the Y axis. sinVal and cosVal are 4.12 fixed point. */
void G3_RotY(int sinVal, int cosVal);
/* Same, about the X axis. */
void G3_RotX(int sinVal, int cosVal);

#endif
