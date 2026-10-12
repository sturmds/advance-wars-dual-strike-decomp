#include "sdk/wl.h"

void func_arm7_027c90ec(u16 *dst, u16 *src);
void Mem_CopyHalfwords_Arm7(const void *src, void *dest, int size);

int WifiCmd_ReportAllSettings(WlRequest *request, WlStatusReply *reply)
{
    reply->status = 33;
    func_arm7_027c90ec(reply->macAddress, WIFI_DRIVER_STATE->macAddress);
    reply->work2fe = WIFI_DRIVER_STATE->unk_2fe;
    reply->state00 = WIFI_DRIVER_STATE->state.unk_00;
    reply->state92 = WIFI_DRIVER_STATE->state.unk_92;
    reply->phase = WIFI_DRIVER_STATE->state.phase;
    reply->state04 = WIFI_DRIVER_STATE->state.unk_04;
    reply->state08 = WIFI_DRIVER_STATE->state.unk_08;
    reply->state0a = WIFI_DRIVER_STATE->state.unk_0a;
    reply->flag0 = WIFI_DRIVER_STATE->state.flags.bit0;
    reply->flag1 = WIFI_DRIVER_STATE->state.flags.bit1;
    reply->state96 = WIFI_DRIVER_STATE->state.unk_96;
    reply->state10 = WIFI_DRIVER_STATE->state.unk_10;
    Mem_CopyHalfwords_Arm7(WIFI_DRIVER_STATE->state.words_58, reply->state58, sizeof(reply->state58));
    reply->flag2 = WIFI_DRIVER_STATE->state.flags.bit2;
    reply->state06 = WIFI_DRIVER_STATE->state.unk_06;
    return 0;
}
