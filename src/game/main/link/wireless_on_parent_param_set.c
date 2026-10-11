#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;

BOOL Wireless_StartParent(void);
void func_02079180(void);

/* Callback of the request that sends the parent settings (func_0207909c).
 * On success runs the next step for the state in data_0229c880: state 4
 * starts the parent first, states 3 and 5 become 3. On an error the state
 * becomes 4 and func_02079180 runs. */
void Wireless_OnParentParamSet(WirelessEvent *event)
{
    if (event->errorCode == 0) {
        switch (data_0229c880) {
        case 0:
            func_02079180();
            return;
        case 4:
            if (Wireless_StartParent()) {
                return;
            }
            func_02079180();
            return;
        case 3:
        case 5:
            data_0229c880 = 3;
            func_02079180();
            return;
        }
    } else {
        data_0229c880 = 4;
        func_02079180();
    }
}
