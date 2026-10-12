#ifndef SDK_MIC_H
#define SDK_MIC_H

#include "types.h"

/*
 * ARM7 side of the microphone. The ARM9 sends a request as PXI words; the
 * ARM7 queues it with the SPI manager, then samples the microphone ADC over
 * SPI on each timer 3 interrupt and writes the samples into a buffer in main
 * RAM. The game links no ARM9 microphone code.
 */

/*
 * A request is up to 16 PXI words. Bits 16-19 of each word give its index,
 * MIC_PXI_START marks the first and MIC_PXI_END the last; the low 16 bits
 * are data. Word 0 holds the command byte in bits 8-15 and the first
 * argument byte in bits 0-7.
 */
#define MIC_PXI_START       0x02000000
#define MIC_PXI_END         0x01000000
#define MIC_PXI_INDEX_MASK  0x000f0000
#define MIC_PXI_INDEX_SHIFT 16
#define MIC_PXI_WORDS       16

/* The sampling buffer must lie in main RAM. */
#define MIC_BUFFER_MIN 0x02000000
#define MIC_BUFFER_END 0x02400000

/* Requests (the command byte of a PXI word) and the buffer-full notice. */
enum {
    MIC_PXI_COMMAND_ONE_SAMPLE = 0x40,
    MIC_PXI_COMMAND_AUTO_START = 0x41,
    MIC_PXI_COMMAND_AUTO_STOP = 0x42,
    MIC_PXI_COMMAND_AUTO_ADJUST = 0x43,
    MIC_PXI_NOTICE_BUFFER_FULL = 0x51
};

/* Result codes in a reply. */
enum {
    MIC_PXI_RESULT_SUCCESS = 0,
    MIC_PXI_RESULT_INVALID_COMMAND = 1,
    MIC_PXI_RESULT_INVALID_PARAMETER = 2,
    MIC_PXI_RESULT_ILLEGAL_STATUS = 3,
    MIC_PXI_RESULT_EXCLUSIVE = 4
};

/* The SPI manager's device number for the microphone. */
#define SERIAL_JOB_MICROPHONE 2

/* Sampling type bits, from the low byte of the request's first word. */
#define MIC_TYPE_12BIT     0x01        /* 12-bit samples in halfwords; otherwise 8-bit, two per halfword */
#define MIC_TYPE_SIGNED    0x02        /* flip the top bit: the ADC gives unsigned samples */
#define MIC_TYPE_NO_REPEAT 0x04        /* store 0xffff, not the last sample, when the SPI bus is busy */
#define MIC_TYPE_LOOP      0x10        /* start over at a full buffer instead of stopping */

enum {
    MIC_STATUS_READY = 0,
    MIC_STATUS_AUTO_START = 1,          /* request taken, sampling not started yet */
    MIC_STATUS_AUTO_SAMPLING = 2,
    MIC_STATUS_AUTO_END = 3,            /* stop requested by the ARM9 */
    MIC_STATUS_END_WAIT = 4             /* buffer full, stop queued */
};

/* Driver state at data_arm7_0380af74. */
typedef struct MICWork {
    u16 command[16];               /* PXI words of the request being received */
    u32 status;                    /* MIC_STATUS_* */
    u16 type;                      /* MIC_TYPE_* bits of the last auto sampling request */
    u16 mode;                      /* type & 7, copied when sampling starts */
    u8 *buffer;
    u32 offset;                    /* bytes written so far */
    u32 size;
    u16 timerReload;
    u16 timerControl;              /* prescaler bits of TM3CNT_H */
    u16 pendingSample;             /* 8-bit mode: even sample waiting for its odd partner */
    u16 lastSample;
} MICWork;

extern MICWork data_arm7_0380af74;

/* Shared main RAM at 0x027ffc00: the ARM9 reads the latest sample here. */
typedef struct MICSystemWork {
    u8 pad_000[0x390];
    void *lastAddress;             /* where the latest sample went in the buffer */
    u16 lastSample;
} MICSystemWork;

#define MIC_SYSTEM_WORK ((MICSystemWork *)0x027ffc00)

/* Timer 3 paces the samples. */
#define REG_TM3CNT_L (*(volatile u16 *)0x0400010c)
#define REG_TM3CNT_H (*(volatile u16 *)0x0400010e)
#define TIMER_PRESCALER_1    0
#define TIMER_PRESCALER_64   1
#define TIMER_PRESCALER_256  2
#define TIMER_PRESCALER_1024 3
#define TIMER_IRQ_ENABLE 0x0040
#define TIMER_ENABLE     0x0080

/* The ARM7 SPI bus (touch panel, firmware flash, power management, microphone). */
#define REG_SPICNT  (*(volatile u16 *)0x040001c0)
#define REG_SPIDATA (*(volatile u16 *)0x040001c2)
#define SPI_BUSY 0x0080
#define SERIAL_RATE_2MHZ  0x0001
#define SERIAL_SELECT_TOUCH_ADC 0x0200          /* touch screen controller; its ADC also reads the microphone */
#define SERIAL_KEEP_CHIP_SELECTED       0x0800          /* keep chip select after this byte */
#define SPI_ENABLE     0x8000

/* Touch screen controller commands that convert the microphone channel. */
#define TSC_READ_MIC_12BIT 0xe4
#define TSC_READ_MIC_8BIT  0xec

/* A request the SPI manager queued, as its thread hands it to the device code. */
typedef struct SerialBusJob {
    u32 device;
    u32 command;                   /* PXI command byte */
    u32 args[4];
} SerialBusJob;

/* ARM7 SPI manager calls (not decompiled yet). */
BOOL SPIi_CheckEntry(void);         /* a request is waiting in the queue */
BOOL func_arm7_03803d38(int device, int command, int argCount, ...);  /* queue a request; FALSE when full */
void func_arm7_03803e0c(int device);   /* release the exclusive lock */
void func_arm7_03803e50(int device);   /* take the exclusive lock */
int func_arm7_03803e68(int device);    /* nonzero when no device holds the lock */
void func_arm7_03803e84(u16 command, u16 result);  /* PXI reply to the ARM9 */

u16 func_arm7_038060bc(void);
u16 func_arm7_03806184(void);

#endif
