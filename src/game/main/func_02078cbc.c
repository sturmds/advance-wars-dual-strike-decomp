#include "types.h"
#include "game/wireless_event.h"
#include "game/Unk_0215fe84.h"

extern u32 data_0229c880;
extern u32 gWirelessState;
int func_02079180(void);
int Wireless_StartParent(void);
int Wireless_StartScan(void);
int Wireless_PrepareScan(void);
void func_02079ee0(void);

void func_02078cbc(WirelessEvent *event)
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
            if (!Wireless_PrepareScan()) {
                func_02079ee0();
                return;
            }
            data_0215fe84->scanning = 1;
            gWirelessState = 3;
            if (Wireless_StartScan()) {
                return;
            }
            data_0229c880 = 3;
            func_02079180();
            return;
        }
    } else {
        data_0229c880 = 3;
        func_02079180();
    }
}
