#include "sdk/tp.h"
#include "sdk/os.h"

/* Unpacks the sample the ARM7 left in shared memory. */
static inline void TPi_CopyTpFromSystemWork(TPData *tp)
{
    SPITpData spi;

    spi.halfs[0] = TOUCH_SHARED_LATEST_LOW;
    spi.halfs[1] = TOUCH_SHARED_LATEST_HIGH;
    tp->x = (u16)spi.e.x;
    tp->y = (u16)spi.e.y;
    tp->touch = (u8)spi.e.touch;
    tp->validity = (u8)spi.e.validity;
}

/*
 * PXI receive callback for the touch panel tag. Handles three kinds of
 * word from the ARM7: a send error, a sample from auto sampling (stored in
 * the next ring buffer slot), and the reply to a request, which clears the
 * request's pending bit and on failure sets its error bit. The user
 * callback, if any, hears about each.
 */
void TPi_TpCallback(u32 tag, u32 data, int err)
{
    u16 data16 = (u16)data;
    u16 command = (u16)((data16 & TOUCH_REPLY_REQUEST_BITS) >> TOUCH_REPLY_REQUEST_POS);
    int result;

    if (err) {
        tpState.err_flg |= 1 << command;
        if (tpState.callback != NULL) {
            tpState.callback(command, TP_RESULT_PXI_BUSY, 0);
        }
        return;
    }

    if (command == TP_REQUEST_COMMAND_AUTO_SAMPLING) {
        tpState.index++;
        if (tpState.index >= tpState.bufSize) {
            tpState.index = 0;
        }
        TPi_CopyTpFromSystemWork(&tpState.samplingBufs[tpState.index]);
        if (tpState.callback != NULL) {
            tpState.callback(command, TP_RESULT_SUCCESS, (u8)tpState.index);
        }
        return;
    }

    if (!(data & TOUCH_REPLY_ANSWER_BIT)) {
        return;
    }

    switch ((u8)(data16 & TOUCH_REPLY_CODE_BITS)) {
    case TOUCH_REPLY_OK:
        switch (command) {
        case TP_REQUEST_COMMAND_SAMPLING:
            TPi_CopyTpFromSystemWork(&tpState.buf);
            tpState.state = TP_STATE_READY;
            break;
        case TP_REQUEST_COMMAND_AUTO_ON:
            tpState.state = TP_STATE_AUTO_SAMPLING;
            break;
        case TP_REQUEST_COMMAND_AUTO_OFF:
            tpState.state = TP_STATE_READY;
            break;
        }
        tpState.command_flg &= ~(1 << command);
        if (tpState.callback != NULL) {
            tpState.callback(command, TP_RESULT_SUCCESS, 0);
        }
        return;
    case TOUCH_REPLY_BUS_TAKEN:
        result = TP_RESULT_EXCLUSIVE;
        break;
    case TOUCH_REPLY_BAD_ARGUMENT:
        result = TP_RESULT_INVALID_PARAMETER;
        break;
    case TOUCH_REPLY_WRONG_STATE:
        result = TP_RESULT_ILLEGAL_STATUS;
        break;
    default:
        goto unknown;           /* needed to match: OS_Terminate comes last */
    }

    tpState.err_flg |= 1 << command;
    tpState.command_flg &= ~(1 << command);
    if (tpState.callback != NULL) {
        tpState.callback(command, result, 0);
    }
    return;

unknown:
    OS_Terminate();
}
