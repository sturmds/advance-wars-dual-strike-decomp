#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/Unk_0229c960.h"
#include "sdk/wm.h"

extern u32 data_0229c880;
extern WMBssDesc *data_0229c8a8;

void func_0210e100(void *start, u32 size);
int func_02079180(void);
int func_02078864(void);
int func_02079bb0(void);
void func_02079ee0(void);
void Wireless_AddParent(u8 linkLevel);
BOOL Wireless_StartRequest(void);

/* Scan callback (Wireless_StartRequest passes it to the SDK). In state 3 a
 * found parent with our game group id and no download play flag goes to
 * Wireless_AddParent, then the scan restarts while data_0215fe84->unk_14 is set. */
void Wireless_OnScan(WMStartScanCallback *cb)
{
    if (cb->errcode == 0) {
        switch (data_0229c880) {
        case 0:
            func_02079180();
            return;
        case 4:
            if (func_02078864()) {
                return;
            }
            func_02079180();
            return;
        case 5:
            if (func_02078864()) {
                return;
            }
            func_02079180();
            return;
        case 3:
            switch (cb->state) {
            case WM_STATECODE_PARENT_FOUND:
                func_0210e100(data_0229c8a8, sizeof(WMBssDesc));
                /* the beacon must reach past gameInfo.ggid (0x48 bytes) */
                if (data_0229c8a8->length * 2 >= 0x48
                    && data_0229c8a8->gameInfo.ggid == data_0229c960.ggid
                    && !(data_0229c8a8->gameInfo.attribute & WM_ATTR_FLAG_MB)) {
                    Wireless_AddParent(cb->linkLevel);
                }
                /* fall through */
            case WM_STATECODE_PARENT_NOT_FOUND:
                if (data_0215fe84->unk_14 == 0) {
                    return;
                }
                if (!func_02079bb0()) {
                    func_02079ee0();
                    return;
                }
                if (Wireless_StartRequest()) {
                    return;
                }
                func_02079180();
                return;
            default:
                data_0229c880 = 0;
                func_02079180();
                return;
            }
        }
    } else {
        data_0229c880 = 3;
        func_02079180();
    }
}
