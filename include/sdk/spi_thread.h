#ifndef SDK_SPI_THREAD_H
#define SDK_SPI_THREAD_H

#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/spi.h"

/* The SPI thread: it takes jobs from a message queue and runs them one at a
 * time. A device holds the bus while it runs a job; SPI_Lock lets other ARM7
 * code take it too. SerialBusThreadState holds an ARM7 OSThread, so it is kept out of
 * sdk/spi.h, which the ARM9 TP code includes. */

#define SERIAL_THREAD_STACK_BYTES 0x200
#define SERIAL_JOB_QUEUE_DEPTH 16           /* jobs waiting at once */

typedef struct SerialBusThreadState {
    BOOL busLocked;                       /* 0x000 a device holds the bus */
    u32 lockDevice;                       /* 0x004 SERIAL_OWNER_*: who holds it */
    OSThread thread;                      /* 0x008 */
    u32 stack[SERIAL_THREAD_STACK_BYTES / 4]; /* 0x080 */
    OSMessageQueue msgQ;                  /* 0x280 each message points to an entry */
    OSMessage msgArray[SERIAL_JOB_QUEUE_DEPTH];    /* 0x294 */
    SerialBusJob entry[SERIAL_JOB_QUEUE_DEPTH];        /* 0x2d4 ring of job slots */
    u32 entryIndex;                       /* 0x454 next free slot */
    OSThreadQueue waitQueue;              /* 0x458 threads waiting in SPI_Lock */
    u32 lockId;                           /* 0x45c the SPI_Lock caller's id */
} SerialBusThreadState;

extern SerialBusThreadState data_arm7_03809ab0;

#endif
