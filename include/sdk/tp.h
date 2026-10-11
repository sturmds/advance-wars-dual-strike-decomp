#ifndef SDK_TP_H
#define SDK_TP_H

#include "types.h"

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

typedef void (*TPRecvCallback)(u32 command, u32 result, u16 index);

typedef struct TPiWork {
    TPRecvCallback callback;
    TPData buf;                    /* latest single sample */
    u16 index;                     /* latest slot written in samplingBufs */
    u16 frequency;
    TPData *samplingBufs;          /* ring buffer for auto sampling */
    u16 bufSize;
    u16 pad_16;
    TPiCalibrateParam calibrate;
    u16 calibrate_flg;             /* TRUE once TP_SetCalibrateParam got a param */
    u16 state;                     /* TOUCH_CLIENT_* */
    volatile u16 err_flg;          /* one bit per request: it failed */
    volatile u16 command_flg;      /* one bit per request: waiting for the ARM7 */
} TPiWork;

extern TPiWork tpState;

/* PXI FIFO tag of the touch panel driver. */
#define FIFO_TAG_TOUCH 6

/* Ask the ARM7 for one sample. */
#define TOUCH_ASK_ONE_SAMPLE 0x03000000

void TP_Init(void);
BOOL TP_GetUserInfo(TPCalibrateParam *calibrate);
void TP_SetCalibrateParam(const TPCalibrateParam *param);
void TP_RequestSamplingAsync(void);
u32 TP_WaitRawResult(TPData *result);
void TP_GetCalibratedPoint(TPData *disp, const TPData *raw);
void TP_WaitBusy(u32 commandFlags);
u32 TP_CalcCalibrateParam(TPCalibrateParam *calibrate, u16 rawX1, u16 rawY1, u16 dx1, u16 dy1,
                          u16 rawX2, u16 rawY2, u16 dx2, u16 dy2);

#endif
