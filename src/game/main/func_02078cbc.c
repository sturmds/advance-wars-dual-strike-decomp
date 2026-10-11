#include "types.h"
#include "game/Unk_0215fe84.h"

typedef struct WirelessEvent {
    u16 apiId;
    u16 errorCode;
} WirelessEvent;
extern u32 data_0229c880;
extern u32 data_0229c88c;
int func_02079180(void);
int func_02078dc0(void);
int Wireless_StartRequest(void);
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
            if (func_02078dc0()) {
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
            data_0215fe84->unk_14 = 1;
            data_0229c88c = 3;
            if (Wireless_StartRequest()) {
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
