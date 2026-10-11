#include "sdk/tp.h"
#include "sdk/cp.h"
#include "sdk/os.h"

/* Stores a calibration for TP_GetCalibratedPoint, with 1 / dot size worked
 * out once on the hardware divider. NULL turns calibration off. */
void TP_SetCalibrateParam(const TPCalibrateParam *param)
{
    int enabled;

    if (param == NULL) {
        tpState.calibrate_flg = FALSE;
        return;
    }

    enabled = OS_DisableInterrupts();

    CP_SetDiv32_32(1 << 28, param->xDotSize);
    tpState.calibrate.x0 = param->x0;
    tpState.calibrate.xDotSize = param->xDotSize;
    CP_WaitDiv();
    tpState.calibrate.xDotSizeInv = CP_GetDivResult32();

    CP_SetDiv32_32(1 << 28, param->yDotSize);
    tpState.calibrate.y0 = param->y0;
    tpState.calibrate.yDotSize = param->yDotSize;
    CP_WaitDiv();
    tpState.calibrate.yDotSizeInv = CP_GetDivResult32();

    OS_RestoreInterrupts(enabled);
    tpState.calibrate_flg = TRUE;
}
