#include "sdk/mi.h"
#include "sdk/wl.h"

void func_arm7_037f8e04(void);
void func_arm7_037f92c8(void);
u16 func_arm7_027c8bcc(int reg);
int func_arm7_027c8b8c(int reg, int value);
void func_arm7_027c963c(int channel, int arg1);
void func_arm7_027c87b8(int a0);

// Builds a frame of 0x5555 halfwords in WIFI_PACKET_RAM and starts sending it (signal
// 3 or 4; signal 4 also clears baseband register 6 until the test stops).
void WifiTest_SendPatternFrames(WlTestModeRequest *request)
{
    WlLink *link = WIFI_LINK_VIEW_OF(WIFI_DRIVER_STATE);
    volatile u16 *ram = WIFI_PACKET_RAM; /* needed to match: keeps the last line unfolded */
    u32 i;
    volatile u16 *p;

    func_arm7_037f8e04();
    func_arm7_037f92c8();
    link->savedBbReg6 = func_arm7_027c8bcc(6);
    if (request->signal == 4) {
        func_arm7_027c8b8c(6, 0);
    }
    {
        volatile u16 zero = 0;
        Mem_FillHalfwords_Arm7(zero, (void *)ram, 12);
    }
    ram[4] = 20;
    ram[5] = 2000;
    p = &ram[6];
    for (i = 0; i < 0x5f8; i += 2) {
        *p++ = 0x5555;
    }
    ram[6] = 8;
    WIFI_TX_HEADER_CONTROL = 6;
    link->mode = WIFI_MODE_FRAME_TEST;
    link->testTxActive = 1;
    func_arm7_027c963c(request->channel, 1);
    WIFI_MAC_FORCE_POWER = 0x8000;
    func_arm7_027c87b8(1500);
    WIFI_IRQ_ENABLE_MASK = 2;
    WIFI_MAC_RUN = 1;
    WIFI_TX_REQUEST_RAISE = 1;
    WIFI_TX_LOC1_FRAME_ADDR = 0x8000 | (u16)(((u32)ram & 0x3fff) >> 1);
}
