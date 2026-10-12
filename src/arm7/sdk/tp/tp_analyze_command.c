#include "types.h"
#include "sdk/tp.h"

/* Takes the touch panel words from the SPI PXI handler: collects the words of a
 * request, then checks it and queues it for the SPI thread or answers it. */
void TP_AnalyzeCommand(u32 data)
{
    u16 command;
    u16 frequency;
    u16 vCount;

    if (data & TOUCH_MSG_FIRST_WORD) {
        int i;
        for (i = 0; i < TOUCH_MSG_MAX_HALFWORDS; i++) {
            tpWork.command[i] = 0;
        }
    }

    tpWork.command[(data & TOUCH_MSG_SLOT_BITS) >> TOUCH_MSG_SLOT_POS] = data;

    if (!(data & TOUCH_MSG_LAST_WORD)) {
        return;
    }

    command = (tpWork.command[0] & 0xff00) >> 8;
    switch (command) {
    case TP_REQUEST_COMMAND_SET_STABILITY:
        Touch_ApplyStabilityRequest(tpWork.command[0] & 0xff);
        break;

    case TP_REQUEST_COMMAND_SAMPLING:
        if (!func_arm7_03803d38(SERIAL_OWNER_TOUCH, command, 0)) {
            func_arm7_03803e84(command, TOUCH_REPLY_BUS_TAKEN);
        }
        break;

    case TP_REQUEST_COMMAND_AUTO_ON:
        if (tpWork.status != TOUCH_SAMPLER_IDLE) {
            func_arm7_03803e84(command, TOUCH_REPLY_WRONG_STATE);
            return;
        }
        frequency = tpWork.command[0] & 0xff;
        if (frequency == 0 || frequency > TOUCH_MAX_SAMPLES_PER_FRAME) {
            func_arm7_03803e84(command, TOUCH_REPLY_BAD_ARGUMENT);
            return;
        }
        vCount = tpWork.command[1];
        if (vCount >= TOUCH_SCANLINES_PER_FRAME) {
            func_arm7_03803e84(command, TOUCH_REPLY_BAD_ARGUMENT);
            return;
        }
        if (!func_arm7_03803d38(SERIAL_OWNER_TOUCH, command, 2, frequency, vCount)) {
            func_arm7_03803e84(command, TOUCH_REPLY_BUS_TAKEN);
            return;
        }
        tpWork.status = TOUCH_SAMPLER_ALARMS_PENDING;
        break;

    case TP_REQUEST_COMMAND_AUTO_OFF:
        if (tpWork.status != TOUCH_SAMPLER_PERIODIC) {
            func_arm7_03803e84(command, TOUCH_REPLY_WRONG_STATE);
            return;
        }
        if (!func_arm7_03803d38(SERIAL_OWNER_TOUCH, command, 0)) {
            func_arm7_03803e84(command, TOUCH_REPLY_BUS_TAKEN);
            return;
        }
        tpWork.status = TOUCH_SAMPLER_STOPPING;
        break;

    default:
        func_arm7_03803e84(command, TOUCH_REPLY_UNKNOWN_REQUEST);
        break;
    }
}
