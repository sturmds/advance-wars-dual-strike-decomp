#ifndef SDK_TP_H
#define SDK_TP_H

#include "types.h"
#include "sdk/os_common.h"
#include "sdk/spi.h"

/* One touch panel reading. Raw readings are 12-bit panel units;
 * TP_GetCalibratedPoint turns them into screen pixels. The game's
 * TouchSample (game/touch.h) has the same layout. */
typedef struct TPData {
    u16 x;
    u16 y;
    u16 touch;                     /* PEN_* */
    u16 validity;                  /* TOUCH_READING_* bits; 0 when both axes are good */
} TPData;

#define TP_TOUCH_OFF 0
#define TP_TOUCH_ON  1

/* Calibration as the user sets it: the raw reading at screen pixel 0 times 4,
 * and the raw units per screen pixel times 256 (see TP_CalcCalibrateParam). */
typedef struct TPCalibrateParam {
    s16 x0;
    s16 y0;
    s16 xDotSize;
    s16 yDotSize;
} TPCalibrateParam;

/* The driver's working copy of the calibration, with 1 / dot size kept in
 * fixed point so a conversion needs no division. */
typedef struct TPiCalibrateParam {
    s32 x0;
    s32 xDotSize;
    s32 xDotSizeInv;               /* (1 << 28) / xDotSize */
    s32 y0;
    s32 yDotSize;
    s32 yDotSizeInv;
} TPiCalibrateParam;

/* Requests to the ARM7 touch driver; bit n of the TPiWork flag words
 * belongs to request n. */
enum {
    TP_REQUEST_COMMAND_SAMPLING = 0,
    TP_REQUEST_COMMAND_AUTO_ON = 1,
    TP_REQUEST_COMMAND_AUTO_OFF = 2,
    TP_REQUEST_COMMAND_SET_STABILITY = 3,
    TP_REQUEST_COMMAND_AUTO_SAMPLING = 0x10  /* a sample from auto sampling */
};
#define TOUCH_JOB_ONE_SAMPLE_BIT (1 << TP_REQUEST_COMMAND_SAMPLING)

/* Results passed to TPRecvCallback. */
enum {
    TP_RESULT_SUCCESS = 0,
    TP_RESULT_INVALID_PARAMETER = 1,
    TP_RESULT_ILLEGAL_STATUS = 2,
    TP_RESULT_EXCLUSIVE = 3,
    TP_RESULT_PXI_BUSY = 4
};

/* TPiWork.state */
enum {
    TP_STATE_READY = 0,
    TP_STATE_AUTO_SAMPLING = 2
};

typedef void (*TPRecvCallback)(u32 command, u8 result, u16 index);

typedef struct TPiWork {
    TPRecvCallback callback;
    TPData buf;                    /* latest single sample */
    volatile u16 index;            /* latest slot written in samplingBufs */
    u16 frequency;
    TPData *samplingBufs;          /* ring buffer for auto sampling */
    u16 bufSize;
    u16 pad_16;
    TPiCalibrateParam calibrate;
    u16 calibrate_flg;             /* TRUE once TP_SetCalibrateParam got a param */
    volatile u16 state;            /* TOUCH_CLIENT_* */
    volatile u16 err_flg;          /* one bit per request: it failed */
    volatile u16 command_flg;      /* one bit per request: waiting for the ARM7 */
} TPiWork;

extern TPiWork tpState;

/* Ask the ARM7 for one sample. */
#define TOUCH_ASK_ONE_SAMPLE 0x03000000

/* Fields of a word the ARM7 sends back: the request it answers, a result
 * code, and a flag set on a reply to a request. */
#define TOUCH_REPLY_REQUEST_BITS  0x7f00
#define TOUCH_REPLY_REQUEST_POS 8
#define TOUCH_REPLY_CODE_BITS   0xff
#define TOUCH_REPLY_ANSWER_BIT    0x01000000

/* Result codes in an ARM7 reply. */
enum {
    TOUCH_REPLY_OK = 0,
    TOUCH_REPLY_UNKNOWN_REQUEST = 1,
    TOUCH_REPLY_BAD_ARGUMENT = 2,
    TOUCH_REPLY_WRONG_STATE = 3,
    TOUCH_REPLY_BUS_TAKEN = 4
};

/* The ARM7 leaves its latest sample here, as two halfwords of one SPITpData. */
#define TOUCH_SHARED_LATEST_LOW (*(u16 *)0x027fffaa)
#define TOUCH_SHARED_LATEST_HIGH (*(u16 *)0x027fffac)

/* A sample as the ARM7 packs it into 32 bits. */
typedef union SPITpData {
    struct {
        u32 x : 12;
        u32 y : 12;
        u32 touch : 1;
        u32 validity : 2;
        u32 : 5;
    } e;
    u32 raw;
    u16 halfs[2];
} SPITpData;

void TP_Init(void);
BOOL TP_GetUserInfo(TPCalibrateParam *calibrate);
void TP_SetCalibrateParam(const TPCalibrateParam *param);
void TP_RequestSamplingAsync(void);
u32 TP_WaitRawResult(TPData *result);
void TP_GetCalibratedPoint(TPData *disp, const TPData *raw);
void TP_WaitBusy(u32 commandFlags);
u32 TP_CalcCalibrateParam(TPCalibrateParam *calibrate, u16 rawX1, u16 rawY1, u16 dx1, u16 dy1,
                          u16 rawX2, u16 rawY2, u16 dx2, u16 dy2);

/* ARM7 side: the touch panel driver that answers the requests above. */

#define TOUCH_MSG_MAX_HALFWORDS 16           /* halfwords in one request */
#define TOUCH_MAX_SAMPLES_PER_FRAME 4    /* auto samples per frame */
#define TOUCH_SCANLINES_PER_FRAME 263               /* V count lines per frame, blanking included */
#define TOUCH_LINE_ALARM_ID 0x54505641       /* 'TPVA': tag of the auto sampling V alarms */
#define TOUCH_LINE_ALARM_SLACK 10

/* A request from the ARM9 arrives as up to TOUCH_MSG_MAX_HALFWORDS PXI words.
 * Each holds one halfword and its index; command[0] holds the request id
 * in its high byte. */
#define TOUCH_MSG_FIRST_WORD   0x02000000  /* first word of a request */
#define TOUCH_MSG_LAST_WORD     0x01000000  /* last word: the request is complete */
#define TOUCH_MSG_SLOT_BITS  0x000f0000
#define TOUCH_MSG_SLOT_POS 16

/* TouchSamplerState.status */
enum {
    TOUCH_SAMPLER_IDLE = 0,
    TOUCH_SAMPLER_ALARMS_PENDING = 1,      /* auto sampling accepted, alarms not set yet */
    TOUCH_SAMPLER_PERIODIC = 2,
    TOUCH_SAMPLER_STOPPING = 3    /* stop accepted, alarms not cancelled yet */
};

typedef struct TouchSamplerState {
    u16 command[TOUCH_MSG_MAX_HALFWORDS]; /* the request being received */
    u32 status;                     /* TOUCH_SAMPLER_* */
    s32 range;                      /* readings this close count as one point */
    s32 rangeMin;                   /* range never adjusts below this */
    OSVAlarm vAlarm[TOUCH_MAX_SAMPLES_PER_FRAME];
    u16 vCount[TOUCH_MAX_SAMPLES_PER_FRAME];
} TouchSamplerState;

extern TouchSamplerState tpWork;
extern u16 tpLastTouch;         /* pen was down at the last sample */

/* SPICNT values (sdk/spi.h has the registers). Enabled, 2 MHz, touch panel selected; HOLD keeps the device selected
 * after the byte (a continuous transfer). */
#define SPICNT_TP_1BYTE      0x8201
#define SPICNT_TP_CONTINUOUS 0x8a01

/* Controller command: power down between conversions and enable the pen
 * interrupt, so the pen-down line can be read. */
#define TOUCH_CTRL_PEN_IRQ_MODE 0x84

static inline void Touch_WaitBusIdle(void)
{
    while (REG_SPICNT & SPICNT_BUSY) {
    }
}

/* Sends one byte and waits until it is out. */
static inline void Touch_ShiftOutByte(u16 data)
{
    REG_SPIDATA = (u16)(data & 0xff);
    Touch_WaitBusIdle();
}

/* The byte that came in during the last transfer. */
static inline u16 Touch_LastByteIn(void)
{
    return (u16)(REG_SPIDATA & 0xff);
}

/* Pen state as the ARM7 sampler reads it; TP_TOUCH_OFF and TP_TOUCH_ON
 * come first. The pen was down, the line now reads up, and a second look
 * says down again: */
#define PEN_UP_GLITCH 2

/* Axes the sampler measures. */
enum {
    TOUCH_MEASURE_HORIZONTAL = 0,
    TOUCH_MEASURE_VERTICAL = 1
};

/* TPData.validity and SPITpData.e.validity bits */
#define TP_VALIDITY_VALID     0
#define TP_VALIDITY_INVALID_X 1
#define TP_VALIDITY_INVALID_Y 2
#define TP_VALIDITY_INVALID_XY (TP_VALIDITY_INVALID_X | TP_VALIDITY_INVALID_Y)

void Touch_ResetDriver_Arm7(void);
void TP_AnalyzeCommand(u32 data);
void TP_ExecuteProcess(SerialBusJob *entry);
void Touch_ApplyStabilityRequest(u16 range);
void Touch_QueueAutoSample(void *arg);
void Touch_TuneStabilityRange(SPITpData *sample, u16 density);
void TP_ExecSampling(SPITpData *data, s32 range, u16 *density);
u32 Touch_MeasureAxis(u16 *pos, s32 range, int axis, u16 *density);
u32 Touch_ReadPenLine(void);   /* TP_TOUCH_OFF, TP_TOUCH_ON or PEN_UP_GLITCH */

#endif
