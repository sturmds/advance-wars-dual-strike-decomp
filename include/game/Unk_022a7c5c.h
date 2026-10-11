#ifndef GAME_UNK_022A7C5C_H
#define GAME_UNK_022A7C5C_H

#include "types.h"

/* Object reached through data_022a7c5c (main bss); only the fields the
 * overlay 0 show tasks read are listed. */
typedef struct Unk_022a7c5c {
    u32 unk_00;                        /* base of the texture offsets added to tile numbers by MapTile3d_DrawQuadA */
    u32 mapTexOffset;                  /* texture image offset of the map tiles; army sprites at + 0xa00 (MapGfx_LoadTextures) */
    u32 mapTex2Offset;                 /* texture image offset of a second map file (MapGfx_LoadTextures) */
    u8 *animDst;                       /* where MapAnim_QueueFramePart queues the animated map graphics */
    u8 *animFrames[4];                 /* frames of those graphics, 0x3180 bytes each */
    u32 texPaletteOffset;              /* where the map palette goes in texture palette VRAM */
    u8 *anim2Frames[3];                /* frames of a second animation, 0x4400 bytes each (MapAnim_QueueSecondFramePart) */
    u8 *anim2Dst;                      /* where MapAnim_QueueSecondFramePart queues them */
    u8 *unk_34;                        /* base that func_020f8534 and func_020f83d8 add palette offsets to */
    u32 unk_38;                        /* passed to SpriteCells_Draw3d by RangeSpread_Update */
    u32 unk_3c;
    u32 unk_40;                        /* tile base for the sprites Sprite3d_DrawAnimation draws */
    u8 *unk_44;                        /* destination of the colour func_020f8344 queues (+ 8) */
    u32 unk_48;                        /* tile base of the move path arrow cells, 32 bytes per cell (MovePath_DrawArrows3d) */
    u32 unk_4c;                        /* passed with unk_48 to SpriteCells_Draw3d */
    u8 pad_50[0x5c - 0x50];
    u32 unk_5c;                        /* picked for an odd player slot (ShowTask_GetSlotValue) */
    u32 unk_60;                        /* picked for an even player slot */
    u8 pad_64[0x68 - 0x64];
    void *unk_68;                      /* destination of the graphics func_020d8fac loads */
    void *unk_6c;                      /* destination of its palette */
} Unk_022a7c5c;

extern Unk_022a7c5c *data_022a7c5c;

#endif
