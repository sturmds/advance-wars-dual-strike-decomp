#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;
extern u32 data_0229c88c;

void Wireless_RequestChildRole(void);
void Wireless_RequestParentRole(void);

/* Callback of the request in func_02079578. On success clears data_0229c88c
 * and, for modes 3 to 5, runs the follow-up call for the mode; on an error
 * sets data_0229c88c to 1. */
void Wireless_OnStepDone(WirelessEvent *event)
{
    if (event->errorCode == 0) {
        switch (data_0229c880) {
        case 0:
            data_0229c88c = 0;
            return;
        case 4:
            data_0229c88c = 0;
            Wireless_RequestParentRole();
            return;
        case 3:
        case 5:
            data_0229c88c = 0;
            Wireless_RequestChildRole();
            return;
        }
    } else {
        data_0229c88c = 1;
    }
}
