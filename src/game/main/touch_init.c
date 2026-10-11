#include "types.h"
#include "game/touch.h"

extern u32 gTouchDriverState[6];

void func_02114554(void);
u32 func_021144c8(u32 *a);
void func_021143c4(u32 *a);

void Touch_Init(void)
{
    func_02114554();
    if (func_021144c8(gTouchDriverState)) {
        func_021143c4(gTouchDriverState);
    }
    gTouch.phase = TOUCH_RELEASED;
    gTouch.prevPhase = 0;
    gTouch.active = 0;
    gTouch.pressFrames = 0;
    gTouch.holdDelay = 20;
}
