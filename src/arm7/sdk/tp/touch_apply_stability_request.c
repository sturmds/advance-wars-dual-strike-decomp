#include "types.h"
#include "sdk/tp.h"

/* Answers TP_REQUEST_COMMAND_SET_STABILITY: readings within range of each
 * other count as one point. Zero is refused. */
void Touch_ApplyStabilityRequest(u16 range)
{
    if (range == 0) {
        SPIi_ReturnResult(TP_REQUEST_COMMAND_SET_STABILITY, TOUCH_REPLY_BAD_ARGUMENT);
        return;
    }
    tpWork.range = range;
    tpWork.rangeMin = range;
    SPIi_ReturnResult(TP_REQUEST_COMMAND_SET_STABILITY, TOUCH_REPLY_OK);
}
