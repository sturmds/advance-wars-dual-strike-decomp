#include "types.h"
#include "game/Unk_0215fe84.h"
#include "sdk/wm.h"

extern u32 data_0229c880;
extern u32 data_0229c88c;

int func_02079180(void);
BOOL func_02079368(void);
BOOL func_0207930c(void);
void func_02079c64(int arg);
int func_02079bb0(void);
void func_02079ee0(void);
BOOL Wireless_StartRequest(void);

/* Callback of the SDK request func_02079180 starts. On success it picks the
 * next step from data_0229c880; in modes 3 and 5 it restarts the scan. */
void func_020791f8(WMCallback *cb)
{
    if (cb->errcode == 0) {
        data_0215fe84->unk_06 = -1;
        switch (data_0229c880) {
        case 0:
            func_02079368();
            break;
        case 4:
            func_02079c64(1);
            break;
        case 3:
        case 5:
            if (func_0207930c() != TRUE) {
                return;
            }
            if (!func_02079bb0()) {
                func_02079ee0();
                return;
            }
            data_0215fe84->unk_14 = 1;
            data_0229c88c = 3;
            if (Wireless_StartRequest()) {
                return;
            }
            data_0229c880 = 0;
            func_02079180();
            break;
        }
    } else {
        data_0229c88c = 1;
    }
}
