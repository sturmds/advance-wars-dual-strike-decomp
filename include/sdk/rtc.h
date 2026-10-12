#ifndef SDK_RTC_H
#define SDK_RTC_H

#include "types.h"
#include "sdk/os_arm7.h"

/* ARM7 real-time clock. A transfer sends a command byte (mode plus
 * register number in the high nibble) and then moves bytes in or out. */
/* The serial lines: data in bit 0, clock in bit 1, chip select in bit 2;
 * bits 4-6 set the matching line to output. */
#define REG_RTC (*(volatile u16 *)0x04000138)

#define CLOCK_XFER_TO_CHIP 0x06
#define CLOCK_XFER_FROM_CHIP 0x86

#define CLOCK_SEL_STATUS_A 0x00
#define CLOCK_SEL_INT1_SETTING 0x10
#define CLOCK_SEL_DATE_AND_TIME 0x20
#define CLOCK_SEL_ADDR3 0x30
#define CLOCK_SEL_STATUS_B 0x40
#define CLOCK_SEL_INT2_ALARM 0x50
#define CLOCK_SEL_TIME_ONLY 0x60
#define CLOCK_SEL_ADDR7 0x70

/* Register 4 (alarm and interrupt mode): the low nibble selects the
 * INT1 interrupt mode, bit 6 enables the INT2 alarm (cleared when INT2 fires). */
typedef struct RtcIntRegister {
    u16 mode : 4;
    u16 unk_4 : 2;
    u16 int2Enabled : 1;
    u16 test : 1;
    u16 unk_8 : 8;
} RtcIntRegister;

/* Register 0 (status 1). hour24 selects 24-hour mode. powerOn and
 * lowVoltage report a lost clock;
 * int1 and int2 report a fired interrupt. */
typedef struct RtcStatus1 {
    u16 reset : 1;
    u16 hour24 : 1;
    u16 unk_2 : 2;
    u16 int1 : 1;
    u16 int2 : 1;
    u16 lowVoltage : 1;
    u16 powerOn : 1;
    u16 unk_8 : 8;
} RtcStatus1;

/* Register 2: date then time, 7 bytes, every field in BCD. */
typedef struct RtcDateTime {
    u32 year : 8;
    u32 month : 5;
    u32 unk_13 : 3;
    u32 day : 6;
    u32 unk_22 : 2;
    u32 weekday : 3;
    u32 unk_27 : 5;
    u32 unk_4;
} RtcDateTime;

/* An alarm register (command registers 0x10 and 0x50), 3 bytes. The hour
 * is BCD; pm marks an afternoon hour in 12-hour mode and any hour from 12
 * on in 24-hour mode. */
typedef struct RtcAlarm {
    u32 unk_0 : 8;
    u32 hour : 6;
    u32 pm : 1;
    u32 unk_15 : 17;
} RtcAlarm;

/* PXI protocol with the ARM9. A reply word holds the result in bits 0-7,
 * the command it answers in bits 8-14, and bit 15 set. */
#define PXI_FIFO_TAG_RTC 5
#define CLOCK_ANSWER_BIT 0x8000

/* Requests. Reads are 0x1X and writes 0x2X; each moves one register
 * through the request buffer. */
#define CLOCK_ASK_RESTART 0x00
#define CLOCK_ASK_PICK_24H_MODE 0x01
#define CLOCK_ASK_GET_FULL_STAMP 0x10
#define CLOCK_ASK_GET_CALENDAR 0x11
#define CLOCK_ASK_GET_HOURS_MINS 0x12
#define CLOCK_ASK_GET_INT1_RATE 0x13 /* register 1, 1 byte */
#define CLOCK_ASK_GET_ALARM_A 0x14    /* register 1, 3 bytes */
#define CLOCK_ASK_GET_ALARM_B 0x15    /* register 5 */
#define CLOCK_ASK_GET_FLAGS_A 0x16
#define CLOCK_ASK_GET_FLAGS_B 0x17   /* register 4 */
#define CLOCK_ASK_GET_ADDR3 0x18
#define CLOCK_ASK_GET_ADDR7 0x19
#define CLOCK_ASK_PUT_FULL_STAMP 0x20
#define CLOCK_ASK_PUT_CALENDAR 0x21
#define CLOCK_ASK_PUT_HOURS_MINS 0x22
#define CLOCK_ASK_PUT_INT1_RATE 0x23
#define CLOCK_ASK_PUT_ALARM_A 0x24
#define CLOCK_ASK_PUT_ALARM_B 0x25
#define CLOCK_ASK_PUT_FLAGS_A 0x26
#define CLOCK_ASK_PUT_FLAGS_B 0x27
#define CLOCK_ASK_PUT_ADDR3 0x28
#define CLOCK_ASK_PUT_ADDR7 0x29
#define RTC_PXI_COMMAND_INTERRUPT 0x30 /* sent unasked when an alarm fires */

#define RTC_PXI_RESULT_SUCCESS 0
#define RTC_PXI_RESULT_INVALID_COMMAND 1
/* Register 4's interrupt mode does not fit the frequency or alarm request. */
#define CLOCK_FAIL_INT_MODE_MISMATCH 2
#define RTC_PXI_RESULT_BUSY 3
#define RTC_PXI_RESULT_FATAL_ERROR 4

/*
 * The library's statics. The original reaches them all from the queue's
 * address (the busy flag as an offset from it), so one struct describes them.
 */
#define CLOCK_WORKER_STACK_LEN 0x40

typedef struct RtcState {
    OSMessageQueue queue;   /* requests for the RTC thread */
    OSMessage messages[4];
    OSThread thread;
    u32 stack[CLOCK_WORKER_STACK_LEN];
    u32 busy;               /* a request is being handled */
    u16 command;            /* the request being handled */
} RtcState;

extern RtcState data_arm7_027d81b0;

/* A request's data, in shared main memory: the ARM9 fills it before a
 * write and reads it after a read. */
typedef union RtcRequestData {
    RtcDateTime dateTime;
    struct {
        RtcStatus1 status1;
        RtcIntRegister status2;
        u8 value[4]; /* the time, an alarm, the frequency, register 3 or 7 */
    } regs;
} RtcRequestData;

#define CLOCK_SHARED_REQUEST_BUF ((RtcRequestData *)0x027ffde8)

/* Sends `result` for `command` to the ARM9, retrying until the FIFO takes it. */
void func_arm7_027d58a8(int command, int result);

/* Runs one serial transfer of `size` bytes at `data`. */
void func_arm7_027d5b18(int mode, int reg, void *data, int size);

/* Reads the one-byte register 4 into `out`. */
void func_arm7_027d5e98(RtcIntRegister *out);

void func_arm7_037fe3e0(int a0);

#endif
