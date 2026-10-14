#include "types.h"
#include "sdk/wl.h"

void WifiCmd_ApplyAllSettings(void);
void WifiCmd_ArmMpReply(void);
void WifiCmd_EndSession(void);
void WifiCmd_GoIdleMode(void);
void WifiCmd_GoReadyMode(void);
void WifiCmd_PowerDownMode(void);
void WifiCmd_ReadBasebandPair(void);
void WifiCmd_ReportAllSettings(void);
void WifiCmd_ReportAntennaChoice(void);
void WifiCmd_ReportAntennaSwitching(void);
void WifiCmd_ReportAssocListen(void);
void WifiCmd_ReportBeaconForwarding(void);
void WifiCmd_ReportBeaconPeriod(void);
void WifiCmd_ReportBeaconsPerRun(void);
void WifiCmd_ReportBssid(void);
void WifiCmd_ReportChannelInfo(void);
void WifiCmd_ReportExtraData(void);
void WifiCmd_ReportFirmwareInfo(void);
void WifiCmd_ReportListenPeriod(void);
void WifiCmd_ReportMode(void);
void WifiCmd_ReportOpenAuthBan(void);
void WifiCmd_ReportOwnMac(void);
void WifiCmd_ReportPostBeaconTime(void);
void WifiCmd_ReportProbeNeedsSsid(void);
void WifiCmd_ReportRateMode(void);
void WifiCmd_ReportRetryLimit(void);
void WifiCmd_ReportRole(void);
void WifiCmd_ReportShortPreamble(void);
void WifiCmd_ReportSsid(void);
void WifiCmd_ReportSsidHidden(void);
void WifiCmd_ReportSsidMask(void);
void WifiCmd_ReportStationCapacity(void);
void WifiCmd_ReportStatistics(void);
void WifiCmd_ReportWepKeyIndex(void);
void WifiCmd_ReportWepMode(void);
void WifiCmd_ResetDriver(void);
void WifiCmd_ResetTransmitParts(void);
void WifiCmd_RunRadioTest(void);
void WifiCmd_SendDeauth(void);
void WifiCmd_SendDisassoc(void);
void WifiCmd_SendHostFrame(void);
void WifiCmd_SetStationCapacity(void);
void WifiCmd_StartAssociation(void);
void WifiCmd_StartAuthentication(void);
void WifiCmd_StartChannelSurvey(void);
void WifiCmd_StartJoin(void);
void WifiCmd_StartMpPoll(void);
void WifiCmd_StartReassociation(void);
void WifiCmd_StartScan(void);
void WifiCmd_ZeroStatistics(void);
void WifiTx_EnqueueHostData(void);
void func_arm7_027cb9a4(void);
void func_arm7_027cc97c(void);
void func_arm7_027cd2b4(void);
void func_arm7_027cda6c(void);
void func_arm7_027cdadc(void);
void func_arm7_027cdb28(void);
void func_arm7_027cdb6c(void);
void func_arm7_027cdbb0(void);
void func_arm7_027cdbd0(void);
void func_arm7_027cdbe8(void);
void func_arm7_027cdc30(void);
void func_arm7_027cdc78(void);
void func_arm7_027cdd18(void);
void func_arm7_027cde24(void);
void func_arm7_027cde44(void);
void func_arm7_027cde5c(void);
void func_arm7_027cde74(void);
void func_arm7_027cde8c(void);
void func_arm7_027cdea8(void);
void func_arm7_027cdec0(void);
void func_arm7_027cded8(void);
void func_arm7_027cdf20(void);
void func_arm7_027cdf38(void);
void func_arm7_027cdf50(void);
void func_arm7_027cdf68(void);
void func_arm7_027cdf80(void);
void func_arm7_027cdfdc(void);
void func_arm7_027ce024(void);
void func_arm7_027ce03c(void);

WlCommandEntry gWlCommands100[5] = {
#include "data/arm7/gWlCommands100.inc.c"
};
WlCommandEntry gWlCommands2c0[6] = {
#include "data/arm7/gWlCommands2c0.inc.c"
};
WlCommandEntry gWlCommands240[6] = {
#include "data/arm7/gWlCommands240.inc.c"
};
WlCommandEntry gWlCommands300[10] = {
#include "data/arm7/gWlCommands300.inc.c"
};
WlCommandEntry gWlCommands000[11] = {
#include "data/arm7/gWlCommands000.inc.c"
};
WlCommandEntry gWlCommands280[22] = {
#include "data/arm7/gWlCommands280.inc.c"
};
WlCommandEntry gWlCommands200[22] = {
#include "data/arm7/gWlCommands200.inc.c"
};
