#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;
extern u32 gWirelessState;

BOOL func_02079424(void);
BOOL func_02079578(void);

/* Wireless callback: on success, runs the next call for the current state
 * (0, or 3 to 5); on an error code, sets gWirelessState to 1. */
void Wireless_OnEventNextStep(WirelessEvent *event)
{
    if (event->errorCode == 0) {
        switch (data_0229c880) {
        case 0:
            func_02079578();
            break;
        case 3:
        case 4:
        case 5:
            func_02079424();
            break;
        }
    } else {
        gWirelessState = 1;
    }
}
