#include "types.h"
#include "sdk/gx.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280 *data_027c0280;

void Screen_SetGraphicsMode(int dispMode, int bgMode, int bg0Is3d);
void func_0210ba04(void);          /* SDK 3D engine init (G3X_Init?) */
void func_0210a948(void);
void func_0210a980(void);
void func_0210a994(void);
void func_0210a934(void);
void GX_SetBankForTex(int mask);
void GX_SetBankForOBJ(int mask);
void GX_SetBankForBG(int mask);
void func_0210b7d4(const void *table); /* copies 32 colours to the toon table */

/* 32 white colours in ITCM, after the zero block Map3d_LoadTextures clears
 * VRAM with. The original reads them through a plain address with no
 * relocation, so it is a number here. */
#define ITCM_WHITE_TOON_TABLE ((const void *)0x01ffb97c)

/* Sets up the main screen for the 3D map: BG0 shows the 3D engine, layer 0
 * gets priority 3 and layers 1-3 priority 0 in the screen state, the clear
 * colour is blue (0x7c00) and the toon table all white. */
void Map3d_InitScreen(void)
{
    Screen_SetGraphicsMode(1, 0, 1);
    data_027c0280->dispCnt |= 0x100;   /* BG0-BG3 and OBJ on */
    data_027c0280->dispCnt |= 0x200;
    data_027c0280->dispCnt |= 0x400;
    data_027c0280->dispCnt |= 0x800;
    data_027c0280->dispCnt |= 0x1000;
    data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 3;
    data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 0;
    data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 0;
    func_0210ba04();
    G3X_InitMtxStack();
    func_0210a948();
    func_0210a980();
    func_0210a994();
    func_0210a934();
    GX_SetBankForTex(1);
    GX_SetBankForOBJ(16);
    GX_SetBankForBG(2);
    GX_SetBankForTexPltt(32);
    *REG_DISPCNT = (*REG_DISPCNT & ~0x1f00) | 0x1100; /* only BG0 and OBJ on */
    *REG_BG0CNT &= ~3;                                /* BG0 priority 0 */
    *REG_DISP3DCNT &= ~0x3002;                        /* toon shading, acknowledge the overflow flags */
    *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3000) | 0x10; /* anti-aliasing on */
    *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3000) | 0x8;  /* alpha blending on */
    *REG_BLDCNT = 0;
    *REG_SWAP_BUFFERS = 2;             /* auto polygon sort, W buffering */
    G3X_SetClearColor(0x7c00, 0, 0x7fff, 63, FALSE);
    *REG_VIEWPORT = 0xbfff0000;        /* (0, 0) to (255, 191) */
    *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3002) | 0x2; /* highlight shading */
    func_0210b7d4(ITCM_WHITE_TOON_TABLE);
}
