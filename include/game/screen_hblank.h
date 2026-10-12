#ifndef GAME_SCREEN_HBLANK_H
#define GAME_SCREEN_HBLANK_H

#include "types.h"
#include "sdk/gx.h"

/* Main engine register values that Screen_PrepareHBlankRegs packs from the
 * screen state, for the H-blank handlers that split the screen into the
 * normal view and a panel. The volatile ones are needed to match: the
 * handlers load them before reading DISPCNT. */
extern volatile u32 data_027c0270; /* DISPCNT BG/OBJ layer bits */
extern volatile u32 data_027c026c; /* DISPCNT window bits */
extern u32 data_027c0264;          /* BG1OFS */
extern u32 data_027c0274;          /* BG2OFS */
extern u16 data_027c0254;          /* BG1CNT */
extern u16 data_027c0258;          /* BG2CNT */
extern u16 data_027c025c;          /* BG2PA */
extern u16 data_027c0260;          /* BG2PB */
extern u16 data_027c023c;          /* BG2PC */
extern u16 data_027c0248;          /* BG2PD */
extern u16 data_027c0250;          /* BG2X */
extern u16 data_027c0244;          /* BG2Y */
extern u32 data_027c0268;          /* BLDCNT and BLDALPHA */
extern volatile u8 data_027c0238;  /* DISPCNT BG screen offset */

/* Scanline offset of the panel the H-blank handlers show; read again at
 * each use. */
extern volatile s16 data_02297788;

/* DISPCNT setters: layer enables (bits 8-12), window enables (13-15), screen base (27-29). */
static inline void HBlank_SetVisiblePlane(int planes) {
  *REG_DISPCNT = (*REG_DISPCNT & ~0x1f00) | (planes << 8);
}

static inline void HBlank_SetVisibleWnd(int windows) {
  *REG_DISPCNT = (*REG_DISPCNT & ~0xe000) | (windows << 13);
}

static inline void HBlank_SetBgScreenOffset(int offset) {
  *REG_DISPCNT = (*REG_DISPCNT & ~0x38000000) | (offset << 27);
}

#endif
