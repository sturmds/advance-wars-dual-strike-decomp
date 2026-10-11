#include "sdk/tp.h"

#define LCD_WIDTH  256
#define LCD_HEIGHT 192

/* Converts a raw reading to screen pixels with the calibration from
 * TP_SetCalibrateParam, clamped to the screen. Without a calibration the
 * reading is copied unchanged. */
void TP_GetCalibratedPoint(TPData *disp, const TPData *raw)
{
    TPiCalibrateParam *cal;

    if (!tpState.calibrate_flg) {
        *disp = *raw;
        return;
    }

    cal = &tpState.calibrate;
    disp->touch = raw->touch;
    disp->validity = raw->validity;
    if (raw->touch == TP_TOUCH_OFF) {
        disp->x = 0;
        disp->y = 0;
        return;
    }

    disp->x = (s16)((((long long)(raw->x << 2) - cal->x0) * cal->xDotSizeInv) >> 22);
    if ((s16)disp->x < 0) {
        disp->x = 0;
    } else if ((s16)disp->x > LCD_WIDTH - 1) {
        disp->x = LCD_WIDTH - 1;
    }

    disp->y = (s16)((((long long)(raw->y << 2) - cal->y0) * cal->yDotSizeInv) >> 22);
    if ((s16)disp->y < 0) {
        disp->y = 0;
    } else if ((s16)disp->y > LCD_HEIGHT - 1) {
        disp->y = LCD_HEIGHT - 1;
    }
}
