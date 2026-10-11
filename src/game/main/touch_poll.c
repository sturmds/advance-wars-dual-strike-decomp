#include "types.h"
#include "game/touch.h"

extern TouchSample gTouchSample;
extern u32 gTouchRawSample[2];

u32 func_021142cc(u32 *raw);
void func_02114324(void);
void TP_GetCalibratedPoint(TouchSample *sample, u32 *raw);
void Touch_Release(void);
void Touch_UpdateTouched(TouchSample *sample);

/* Reads the touch panel: calls the driver until it returns a raw sample,
 * converts it to screen coordinates, then updates gTouch from it. */
void Touch_Poll(void)
{
    do {
        func_02114324();
    } while (func_021142cc(gTouchRawSample));
    TP_GetCalibratedPoint(&gTouchSample, gTouchRawSample);
    if (gTouchSample.touch != 0 && gTouchSample.touch == 1) {
        Touch_UpdateTouched(&gTouchSample);
    } else {
        Touch_Release();
    }
}
