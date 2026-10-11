#ifndef GAME_DMA_TASK_H
#define GAME_DMA_TASK_H

#include "types.h"
#include "game/task.h"

/* data_0216e054: which DMA channels (0 and 1) the DMA tasks
 * (scripts data_021571ac and data_021571cc) use. */
typedef struct DmaTaskState {
    u8 channelScreen[2];               /* 0 free, 1 started from the main screen, 2 from the sub screen */
    u8 pair;                           /* nonzero: func_0200371c started both tasks together */
    u8 pad_03;
    void (*channel1Callback)(void);    /* the channel 0 task skips its callback when it equals this */
} DmaTaskState;

/* What the creator of a DMA task stores in Task.pad_2c onwards. */
typedef struct DmaTaskArgs {
    const void *src;                   /* table that feeds the register */
    volatile void *dest;               /* I/O register */
    u16 unk_34;
    u16 unk_36;
    void (*callback)(void);            /* called by the task's script command DmaTaskCmd_StartChannel0 */
} DmaTaskArgs;

#define DMA_TASK_ARGS(task) ((DmaTaskArgs *)(task)->pad_2c)

#endif
