#ifndef GAME_UNK_OV005_0236BB80_H
#define GAME_UNK_OV005_0236BB80_H

#include "types.h"

struct Task;
#include "game/task.h"

/* Overlay 5 work block (0xc4 bytes) that func_ov005_02352790 allocates
 * into Ov005Task.work and data_ov005_0236bb80. */
typedef struct Unk_ov005_0236bb80 {
    u32 unk_00;
    int unk_04;                        /* set to 500 by Ov005_SetupScreensWithBlend */
    u32 unk_08;
    fx32 blendEva;                     /* sub screen blendEva and blendEvb as fx32 (0 to 16.0) */
    fx32 blendEvb;
    struct TextPrinter *textPrinter;   /* from Text_PrintByIdOnLayer0 */
    struct Task *fadeTask;             /* from the BrightnessFade_StartOutBlackScreen0 family */
    struct Task *fadeTask2;
    struct Task *songTask;             /* song fade task */
    struct Task *waveTask;             /* runs data_ov005_02368048 (Ov005_UpdateBgWave) */
    BOOL unk_28;                       /* set once the blend-in is done */
    u32 xUsesBufferB;                  /* which of the two x line buffers xLines points at */
    u32 yUsesBufferB;
    int xWaveAmplitude;                /* fx32 pixels */
    int xWaveStep;                     /* fx32 */
    int xWavePhase;                    /* fx32, +1 each frame */
    int yWaveAmplitude;
    int yWaveStep;
    int yWavePhase;
    int *xOffsets;                     /* fx32 offset of each line */
    int *yOffsets;
    s16 *xLines;                       /* per-line offsets the hardware reads */
    s16 *yLines;
    s16 *xBufferA;
    s16 *yBufferA;
    s16 *xBufferB;
    s16 *yBufferB;
    struct Task *textTask;             /* text box task */
    u32 hasTaskA;                      /* nonzero while taskA exists */
    Task *taskA;
    u32 hasTaskB;                      /* nonzero while taskB exists */
    Task *taskB;
    void *block80;                     /* freed last */
    u32 *spriteHandles[4];             /* cleared with SpriteAnim_Free */
    int spritePos[4][2];               /* x, y of each spriteHandles entry */
    void *unk_b4;                      /* file loaded by Ov005_SetupScreensWithAffineBg */
    u8 pad_b8[0xc4 - 0xb8];
} Unk_ov005_0236bb80;

#endif
