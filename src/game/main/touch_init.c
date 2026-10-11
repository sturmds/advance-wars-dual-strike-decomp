#include "types.h"
#include "game/touch.h"

extern u32 gTouchDriverState[6];

void func_02114554(void);
u32 func_021144c8(u32 *a);
void TP_SetCalibrateParam(u32 *a);

void Touch_Init(void)
{
    func_02114554();
    if (func_021144c8(gTouchDriverState)) {
        TP_SetCalibrateParam(gTouchDriverState);
    }
    gTouch.phase = TOUCH_RELEASED;
    gTouch.prevPhase = 0;
    gTouch.active = 0;
    gTouch.pressFrames = 0;
    gTouch.holdDelay = 20;
}
