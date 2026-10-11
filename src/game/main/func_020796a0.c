#include "types.h"
#include "game/wireless_event.h"

extern u32 data_0229c880;
extern u32 data_0229c88c;
BOOL func_02079424(void);
BOOL func_02079578(void);

void func_020796a0(WirelessEvent *event)
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
        data_0229c88c = 1;
    }
}
