#include "types.h"
#include "game/Unk_0215fe84.h"
#include "game/wireless_parent_info.h"
#include "sdk/wm.h"

extern void *data_0229c884;
extern WMBssDesc *data_0229c888;
extern u32 gWirelessState;
extern void *data_0229c890;
extern void *gWirelessSystemBuf;
extern s32 data_0229c89c;
extern s32 data_0229c8a4;
extern WMBssDesc *gWirelessScanBuffer;
extern void *data_0229c8ac;
extern void *data_0229c8b0;
extern WMScanParam *data_0229c8b4;
extern WMBssDesc *gWirelessParents[4];

void *Mem_Alloc(u32 size);
void Wireless_InitParentParam(void);

/* Resets the link state, allocates the wireless buffers (system buffer, parent
 * info, scan request, beacon buffers, data sharing, MP send and receive) and
 * starts the wireless driver on DMA channel 2. The MP send buffer is never
 * smaller than 0x60 bytes. */
void Wireless_Init(void)
{
    int i;

    gWirelessState = 0;
    data_0215fe84->unk_08 = 0;
    data_0215fe84->unk_0c = 1;
    data_0215fe84->dataSharingActive = 0;
    data_0215fe84->scanning = 0;
    data_0215fe84->unk_30 = 0;
    data_0215fe84->unk_06 = -1;
    Wireless_InitParentParam();
    gWirelessSystemBuf = Mem_Alloc(0x1700);
    data_0229c8a0 = Mem_Alloc(0x40);
    data_0229c8b4 = Mem_Alloc(0x20);
    gWirelessScanBuffer = Mem_Alloc(sizeof(WMBssDesc));
    data_0229c888 = Mem_Alloc(sizeof(WMBssDesc));
    for (i = 0; i < 4; i++) {
        gWirelessParents[i] = Mem_Alloc(sizeof(WMBssDesc));
    }
    data_0229c8ac = Mem_Alloc(0x820);
    data_0229c8b0 = Mem_Alloc(0x200);
    data_0229c89c = 0x120;
    data_0229c884 = Mem_Alloc(0x120);
    data_0229c8a4 = 0x940;
    if (data_0229c89c < 0x60) {
        data_0229c89c = 0x60;
    }
    data_0229c890 = Mem_Alloc(0x940);
    WM_Init(gWirelessSystemBuf, 2);
}
