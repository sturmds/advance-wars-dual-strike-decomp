#ifndef GAME_UNK_OV005_0235A30C_H
#define GAME_UNK_OV005_0235A30C_H

#include "types.h"

/* Overlay 5 work block of the task step func_ov005_02359034
 * (Ov005Task.work). At least 0x834c bytes. */
typedef struct Unk_ov005_0235a30c {
    u32 skippedStep;                   /* task step at the moment the player skipped */
    u8 pad_0004[0x8348 - 4];
    void *unk_8348;                    /* passed to Task_Destroy (task system) */
} Unk_ov005_0235a30c;

#endif
