#include "types.h"
#include "sdk/tp.h"

/* Needed to match: with loop invariant code motion on, the compiler swaps
 * the registers of raw[i] and the difference in the last search loop. */
#pragma opt_loop_invariants off

#define TOUCH_CTRL_CONVERT_X 0xd1
#define TOUCH_CTRL_CONVERT_Y 0x91
#define TRY_COUNT 5
/* A conversion comes back as two bytes; the 12-bit value sits in bits 3-14. */
#define TOUCH_ADC_RESULT_BITS  0x7ff8
#define TOUCH_ADC_RESULT_POS 3

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* Measures one axis TRY_COUNT times. If three readings lie within range of
 * the first of them, returns their weighted average (the first counts twice)
 * as valid; otherwise the average of the first and last, marked invalid.
 * density gets the largest spread between any two readings. */
u32 Touch_MeasureAxis(u16 *pos, s32 range, int axis, u16 *density)
{
    s32 i, j, k;
    s32 raw[TRY_COUNT];
    u16 command;
    u32 invalid;
    s32 spread;

    if (axis == TOUCH_MEASURE_HORIZONTAL) {
        command = TOUCH_CTRL_CONVERT_X;
        invalid = TP_VALIDITY_INVALID_X;
    } else {
        command = TOUCH_CTRL_CONVERT_Y;
        invalid = TP_VALIDITY_INVALID_Y;
    }

    Touch_WaitBusIdle();
    REG_SPICNT = SPICNT_TP_CONTINUOUS;
    Touch_ShiftOutByte(command);

    /* sending the next command clocks in the low byte of this result */
    for (i = 0; i < TRY_COUNT; i++) {
        Touch_ShiftOutByte(0);
        raw[i] = Touch_LastByteIn() << 8;
        Touch_ShiftOutByte(command);
        raw[i] |= Touch_LastByteIn();
        raw[i] = (raw[i] & TOUCH_ADC_RESULT_BITS) >> TOUCH_ADC_RESULT_POS;
    }

    REG_SPICNT = SPICNT_TP_1BYTE;
    Touch_ShiftOutByte(0);

    spread = 0;
    for (i = 0; i < TRY_COUNT - 1; i++) {
        for (j = i + 1; j < TRY_COUNT; j++) {
            s32 d = ABS(raw[i] - raw[j]);
            if (d > spread) {
                spread = d;
            }
        }
    }
    *density = spread;

    for (i = 0; i < TRY_COUNT - 2; i++) {
        for (j = i + 1; j < TRY_COUNT - 1; j++) {
            if (ABS(raw[i] - raw[j]) > range) {
                continue;
            }
            for (k = j + 1; k < TRY_COUNT; k++) {
                if (ABS(raw[i] - raw[k]) <= range) {
                    *pos = (((raw[i] << 1) + raw[j] + raw[k]) >> 2) & ~7;
                    return TP_VALIDITY_VALID;
                }
            }
        }
    }

    *pos = ((raw[0] + raw[TRY_COUNT - 1]) >> 1) & ~7;
    return invalid;
}
