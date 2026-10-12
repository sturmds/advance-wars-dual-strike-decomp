#include "types.h"
#include "sdk/tp.h"

void func_arm7_037fdc10(void);                   /* halt: disable interrupts and stop for good */
u32 func_arm7_03804c48(void);
u32 func_arm7_03804a50(u16 *pos, s32 range, int axis, u16 *density);

extern u16 tpLastTouch;

/* Takes one touch panel reading: pen state, then X and Y. density gets the
 * larger spread of the raw readings on either axis, or 0 when the pen is up
 * or the reading is suspect. */
void TP_ExecSampling(SPITpData *data, s32 range, u16 *density)
{
    u16 pos;
    u16 densityX;
    u16 densityY;
    u32 touch;
    int i;

    *density = 0;
    if (range < 0) {
        range = -range;
    }

    touch = func_arm7_03804c48();
    if (touch == TP_TOUCH_OFF) {
        data->e.x = 0;
        data->e.y = 0;
        data->e.touch = TP_TOUCH_OFF;
        data->e.validity = TP_VALIDITY_INVALID_XY;
        tpLastTouch = 0;
        return;
    }

    data->e.validity = func_arm7_03804a50(&pos, range, TOUCH_MEASURE_HORIZONTAL, &densityX);
    data->e.x = pos;
    if (func_arm7_03804a50(&pos, range, TOUCH_MEASURE_VERTICAL, &densityY) == TP_VALIDITY_INVALID_Y) {
        data->e.validity |= TP_VALIDITY_INVALID_Y;
    }
    data->e.y = pos;

    REG_SPICNT = SPICNT_TP_CONTINUOUS;
    for (i = 0; i < 12; i++) {
        Touch_ShiftOutByte(0);
    }
    REG_SPICNT = SPICNT_TP_1BYTE;
    Touch_ShiftOutByte(0);

    if (touch == PEN_UP_GLITCH) {
        data->e.validity = TP_VALIDITY_INVALID_XY;
    }

    touch = func_arm7_03804c48();
    switch (touch) {
    case PEN_UP_GLITCH:
        data->e.touch = TP_TOUCH_ON;
        data->e.validity = TP_VALIDITY_INVALID_XY;
        tpLastTouch = 0;
        break;
    case TP_TOUCH_ON:
        data->e.touch = TP_TOUCH_ON;
        tpLastTouch = 1;
        *density = (densityX >= densityY) ? densityX : densityY;
        break;
    case TP_TOUCH_OFF:
        data->e.touch = TP_TOUCH_OFF;
        tpLastTouch = 0;
        break;
    default:
        func_arm7_037fdc10();
    }
}
