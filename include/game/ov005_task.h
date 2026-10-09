#ifndef GAME_OV005_TASK_H
#define GAME_OV005_TASK_H

#include "types.h"

/* Task used by overlay 5's screens. The first 0x2c bytes belong to the task
 * system (Task_Create). Each screen's first step allocates a work block with
 * Mem_Alloc, stores it in `work` and in an overlay global
 * (data_ov005_0236bb80, data_ov005_0236bbac, ...); its last step frees it. */
typedef struct Ov005Task {
    u8 pad_00[0x2c];
    void *work;
} Ov005Task;

#endif
