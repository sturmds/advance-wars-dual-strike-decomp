#ifndef GAME_UNK_OV005_0236BB80_H
#define GAME_UNK_OV005_0236BB80_H

#include "types.h"

/* Overlay 5 work block (0xc4 bytes) that func_ov005_02352790 allocates
 * into Ov005Task.work and data_ov005_0236bb80. */
typedef struct Unk_ov005_0236bb80 {
    u32 unk_00;
    int unk_04;                        /* set to 500 by Ov005_SetupScreensWithBlend */
    u32 unk_08;
    fx32 blendEva;                     /* sub screen blendEva and blendEvb as fx32 (0 to 16.0) */
    fx32 blendEvb;
    struct ScriptSlot *unk_14;         /* from func_020150f8 */
    u8 pad_18[0x78 - 0x18];
    u32 unk_78;
    u8 pad_7c[0xb4 - 0x7c];
    void *unk_b4;                      /* file loaded by Ov005_SetupScreensWithAffineBg */
    u8 pad_b8[0xc4 - 0xb8];
} Unk_ov005_0236bb80;

#endif
