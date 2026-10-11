#include "types.h"
#include "game/touch.h"

extern u32 gTouchDriverState[6];

void func_02114554(void);
u32 TP_GetUserInfo(u32 *a);
void TP_SetCalibrateParam(u32 *a);

void Touch_Init(void)
{
    func_02114554();
    if (TP_GetUserInfo(gTouchDriverState)) {
        TP_SetCalibrateParam(gTouchDriverState);
    }
    gTouch.phase = TOUCH_RELEASED;
    gTouch.prevPhase = 0;
    gTouch.active = 0;
    gTouch.pressFrames = 0;
    gTouch.holdDelay = 20;
}
