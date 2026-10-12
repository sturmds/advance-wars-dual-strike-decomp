#ifndef SDK_SPI_H
#define SDK_SPI_H

#include "types.h"

/*
 * The ARM7's SPI driver. The touch panel, the firmware flash (NVRAM), the
 * microphone and the power chip share one SPI bus. Their PXI callbacks
 * queue jobs, and one thread (sdk/spi_thread.h) runs them in turn.
 */

/* Devices on the bus, also the job owners in SerialBusJob.device. */
enum {
    SERIAL_OWNER_TOUCH = 0,
    SERIAL_OWNER_FLASH = 1,
    SERIAL_OWNER_MICROPHONE = 2,
    SERIAL_OWNER_POWER_CHIP = 3,
    SERIAL_OWNER_OTHER_ARM7 = 4,      /* other ARM7 code, through SPI_Lock */
    SERIAL_OWNER_NOBODY = 5       /* nobody holds the bus */
};

/* PXI FIFO tags of the SPI devices; the power chip's is PXI_FIFO_TAG_PM in sdk/pm.h. */
#define PXI_FIFO_TAG_NVRAM 4
#define FIFO_TAG_TOUCH 6
#define PXI_FIFO_TAG_MIC 9

/* Request ids are grouped by device: bits 4-6 of the id select it. */
#define SERIAL_REQ_GROUP_BITS 0x70
enum {
    SERIAL_REQ_GROUP_TOUCH = 0x00,     /* 0x00-0x1f */
    SERIAL_REQ_GROUP_FLASH = 0x20,  /* 0x20-0x3f */
    SERIAL_REQ_GROUP_MIC = 0x40,    /* 0x40-0x5f */
    SERIAL_REQ_GROUP_POWER = 0x60      /* 0x60-0x7f */
};

/* A request arrives as up to 16 PXI words, each holding one halfword and
 * its index; the start bit marks the first word and the end bit the last. */
#define SERIAL_MSG_SLOT_BITS  0x000f0000
#define SERIAL_MSG_SLOT_POS 16

/* A reply word: bits 8-15 hold the request id with bit 7 set, bits 0-7
 * the result; the start and end bits mark a one-word message. */
#define SERIAL_MSG_ANSWER_BIT 0x80
#define SERIAL_MSG_FIRST_WORD  0x02000000
#define SPI_PXI_END_BIT    0x01000000

/* Results in a reply word; the ARM9 side reads them as TOUCH_REPLY_*. */
enum {
    SPI_PXI_RESULT_SUCCESS = 0,
    SPI_PXI_RESULT_INVALID_COMMAND = 1,
    SPI_PXI_RESULT_INVALID_PARAMETER = 2,
    SPI_PXI_RESULT_ILLEGAL_STATUS = 3,
    SPI_PXI_RESULT_EXCLUSIVE = 4   /* the bus or the job queue is busy */
};

#define SERIAL_JOB_ARGS 4

/* One job the SPI thread runs: a device, its request id and arguments. */
typedef struct SerialBusJob {
    u32 device;                    /* SERIAL_OWNER_* */
    u32 process;                   /* request id, TOUCH_JOB_* for the touch panel */
    u32 arg[SERIAL_JOB_ARGS];
} SerialBusJob;

void SPI_Init(u32 priority);   /* start the devices and the SPI thread */
void SerialDev_OnFifoMessage(u32 tag, u32 data, int err);
void SerialDev_WorkerLoop(void *arg);
void SPIi_ReturnResult(u16 command, u16 result);    /* send a reply word to the ARM9 */
/* Queue a job for the SPI thread; FALSE when the queue is full. */
BOOL SPIi_SetEntry(u32 device, u32 process, u16 paramCount, ...);
BOOL SPIi_CheckEntry(void);   /* TRUE when a job is queued */
void SPIi_ReleaseException(u32 device);   /* free the bus if device holds it */
void SPI_Unlock(u32 id);   /* give back the bus taken with this id */
void SPI_Lock(u32 id);   /* take the bus, sleeping until it is free */

#endif
