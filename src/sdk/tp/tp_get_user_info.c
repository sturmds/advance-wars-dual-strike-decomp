#include "sdk/tp.h"
#include "sdk/os.h"

/* Builds a calibration from the two touch points saved in the system
 * settings. Returns FALSE when none were saved or they are unusable. */
BOOL TP_GetUserInfo(TPCalibrateParam *calibrate)
{
    /* needed to match: this declaration order gives the original registers */
    u8 dy2;
    u16 rawX2;
    u16 rawY2;
    u8 dy1;
    u16 rawY1;
    u8 dx1;
    u16 rawX1;
    FirmwareUserSettings *info;
    u8 dx2;

    info = FIRMWARE_SETTINGS_COPY;
    rawX1 = info->tpRawX1;
    rawY1 = info->tpRawY1;
    dx1 = info->tpDotX1;
    dy1 = info->tpDotY1;
    rawX2 = info->tpRawX2;
    rawY2 = info->tpRawY2;
    dx2 = info->tpDotX2;
    dy2 = info->tpDotY2;

    if (rawX1 == 0 && rawX2 == 0 && rawY1 == 0 && rawY2 == 0) {
        return FALSE;
    }
    return TP_CalcCalibrateParam(calibrate, rawX1, rawY1, dx1, dy1, rawX2, rawY2, dx2, dy2) == 0;
}
