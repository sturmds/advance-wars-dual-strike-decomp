#ifndef GAME_UNK_022A7C5C_H
#define GAME_UNK_022A7C5C_H

#include "types.h"

/* Object reached through data_022a7c5c (main bss); only the fields the
 * overlay 0 show tasks read are listed. */
typedef struct Unk_022a7c5c {
    u8 pad_00[0x0c];
    u8 *animDst;                       /* where MapAnim_QueueFramePart queues the animated map graphics */
    u8 *animFrames[4];                 /* frames of those graphics, 0x3180 bytes each */
    u32 texPaletteOffset;              /* where the map palette goes in texture palette VRAM */
    u8 *anim2Frames[3];                /* frames of a second animation, 0x4400 bytes each (MapAnim_QueueSecondFramePart) */
    u8 *anim2Dst;                      /* where MapAnim_QueueSecondFramePart queues them */
    u8 pad_34[0x38 - 0x34];
    u32 unk_38;                        /* passed to func_020146f0 by RangeSpread_Update */
    u32 unk_3c;
    u8 pad_40[0x5c - 0x40];
    u32 unk_5c;                        /* picked for an odd player slot (ShowTask_GetSlotValue) */
    u32 unk_60;                        /* picked for an even player slot */
    u8 pad_64[0x68 - 0x64];
    void *unk_68;                      /* destination of the graphics func_020d8fac loads */
    void *unk_6c;                      /* destination of its palette */
} Unk_022a7c5c;

extern Unk_022a7c5c *data_022a7c5c;

#endif
