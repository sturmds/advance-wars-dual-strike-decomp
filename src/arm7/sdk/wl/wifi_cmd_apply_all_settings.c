#include "sdk/wl.h"

int WifiParam_StoreOwnMac(const u16 *macAddress);
int WifiParam_StoreRetryLimit(u16 value);
int WifiParam_StoreAllowedChannels(u16 mask);
int WifiParam_StoreRole(u16 phase);
int WifiParam_StoreRateMode(u32 value);
int WifiParam_StoreWepMode(u32 value);
int func_arm7_027ca130(u32 value);
int WifiRam_StoreFourKeyBlocks(int src);
int WifiParam_SetSsidHidden(u32 enable);
int WifiParam_SetProbeNeedsSsid(u32 enable);
int WifiParam_StoreBeaconsPerRun(u32 value);
// The definition has a third, unused parameter; this caller does not set r2.
int WifiParam_StorePostBeaconTime(u16 value, int writeReg);
int WifiParam_StoreSsidMask(const u16 *words);
int WifiParam_SetShortPreamble(u32 enable);
int func_arm7_027c9e50(int value);

// Sets every parameter while the driver is in mode 16. The result ORs the
// setters' results together.
int WifiCmd_ApplyAllSettings(WlParamsRequest *request, WlReply *reply)
{
    int result;

    reply->status = 1;
    if (WIFI_DRIVER_STATE->state.mode != 16) {
        return 1;
    }
    result = WifiParam_StoreOwnMac(request->macAddress);
    result |= WifiParam_StoreRetryLimit(request->work2fe);
    result |= WifiParam_StoreAllowedChannels(request->mask00);
    result |= WifiParam_StoreRole(request->phase);
    result |= WifiParam_StoreRateMode(request->value04);
    result |= WifiParam_StoreWepMode(request->value08);
    result |= func_arm7_027ca130(request->value0a);
    result |= WifiRam_StoreFourKeyBlocks((int)request->wifiRamBlocks);
    result |= WifiParam_SetSsidHidden(request->optionBit0);
    result |= WifiParam_SetProbeNeedsSsid(request->optionBit1);
    result |= WifiParam_StoreBeaconsPerRun(request->value96);
    result |= WifiParam_StorePostBeaconTime(request->value10, 0);
    result |= WifiParam_StoreSsidMask(request->words58);
    result |= WifiParam_SetShortPreamble(request->optionBit2);
    result |= func_arm7_027c9e50(request->value06);
    return result;
}
