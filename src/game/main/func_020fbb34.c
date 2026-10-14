#include "types.h"
#include "game/Unk_022a7c5c.h"
#include "game/camera_pan.h"
#include "sdk/fx.h"

/* also declared in src/game/itcm/map3d/map3d_draw_tiles.c and 1 more */
u32 data_022a7c58;
Unk_022a7c5c* data_022a7c5c;
/* 0: no range shown; else tiles out of range are this much darker (up to 8) */
int data_022a7c60;
u16 data_022a7c64[4];
MtxFx44 data_022a7cac;
MtxFx44 data_022a7c6c;
Unk_022a7c5c data_022a7cec;
Camera data_022a7d5c;

void func_020fbb34(u32 state) { data_022a7c58 = state; }
