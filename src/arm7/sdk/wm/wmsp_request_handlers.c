#include "types.h"
#include "sdk/wm.h"
#include "sdk/wmsp.h"

void WMSP_Initialize(WmspInitRequest* request);
void WMSP_Reset(void);
void WMSP_End(void);
void WMSP_Enable(WmspInitRequest* request);
void WMSP_Disable(void);
void WMSP_PowerOn(void);
void WMSP_PowerOff(void);
void WMSP_SetParentParam(WmspSetParentParamRequest* request);
void WMSP_StartParent(void);
void WMSP_EndParent(void);
void WMSP_StartScan(WMStartScanReq* req);
void WMSP_EndScan(void);
void WMSP_StartConnect(WMStartConnectReq* req);
void WMSP_Disconnect(WmspRequest* request);
void WMSP_StartMP(WmspStartMPRequest* request);
void WMSP_SetMPData(WmspSetMPDataRequest* request);
void WMSP_EndMP(void);
void WMSP_StartDCF(WmspStartDCFRequest* request);
void WMSP_SetDCFData(WmspSetDCFDataRequest* request);
void WMSP_EndDCF(void);
void WMSP_SetWepKey(WmspSetWepKeyRequest* request);
void func_arm7_027c1cf4(int arg);
void WMSP_SetGameInfo(WmspSetGameInfoRequest* request);
void WMSP_SetBeaconInd(WmspRequest* request);
void WMSP_StartTestMode(void);
void WMSP_StopTestMode(void);
void WMSP_VAlarmMp(WmspRequest* request);
void WMSP_SetLifeTime(WmspSetLifeTimeRequest* request);
void WMSP_MeasureChannel(WMMeasureChannelReq* req);
void WMSP_InitWCounter(void);
void WMSP_GetWCounter(void);
void WMSP_SetEntry(WmspRequest* request);
void WMSP_AutoDeAuth(WmspMacRequest* request);
void func_arm7_027c7984(WmspRequest* request);
void WMSP_SetBeaconPeriod(WmspRequest* request);
void func_arm7_027c7a8c(int a0);
void WMSP_StartScanEx(RadioChannelSweepRequest* req);
void WMSP_SetWepKeyEx(WmspSetWepKeyRequest* request);
void func_arm7_027c5e80(WmspRequest* request);
void func_arm7_027c5e4c(void);
void func_arm7_027c5e10(WmspRequest* request);

/* Indexed by apiid. The three entries after WMSP_SetWepKey do nothing. */
WmspRequestHandler const gWmspRequestHandlers[WMSP_API_COUNT] = {
    (WmspRequestHandler)WMSP_Initialize,
    (WmspRequestHandler)WMSP_Reset,
    (WmspRequestHandler)WMSP_End,
    (WmspRequestHandler)WMSP_Enable,
    (WmspRequestHandler)WMSP_Disable,
    (WmspRequestHandler)WMSP_PowerOn,
    (WmspRequestHandler)WMSP_PowerOff,
    (WmspRequestHandler)WMSP_SetParentParam,
    (WmspRequestHandler)WMSP_StartParent,
    (WmspRequestHandler)WMSP_EndParent,
    (WmspRequestHandler)WMSP_StartScan,
    (WmspRequestHandler)WMSP_EndScan,
    (WmspRequestHandler)WMSP_StartConnect,
    (WmspRequestHandler)WMSP_Disconnect,
    (WmspRequestHandler)WMSP_StartMP,
    (WmspRequestHandler)WMSP_SetMPData,
    (WmspRequestHandler)WMSP_EndMP,
    (WmspRequestHandler)WMSP_StartDCF,
    (WmspRequestHandler)WMSP_SetDCFData,
    (WmspRequestHandler)WMSP_EndDCF,
    (WmspRequestHandler)WMSP_SetWepKey,
    (WmspRequestHandler)func_arm7_027c1cf4,
    (WmspRequestHandler)func_arm7_027c1cf4,
    (WmspRequestHandler)func_arm7_027c1cf4,
    (WmspRequestHandler)WMSP_SetGameInfo,
    (WmspRequestHandler)WMSP_SetBeaconInd,
    (WmspRequestHandler)WMSP_StartTestMode,
    (WmspRequestHandler)WMSP_StopTestMode,
    (WmspRequestHandler)WMSP_VAlarmMp,
    (WmspRequestHandler)WMSP_SetLifeTime,
    (WmspRequestHandler)WMSP_MeasureChannel,
    (WmspRequestHandler)WMSP_InitWCounter,
    (WmspRequestHandler)WMSP_GetWCounter,
    (WmspRequestHandler)WMSP_SetEntry,
    (WmspRequestHandler)WMSP_AutoDeAuth,
    (WmspRequestHandler)func_arm7_027c7984,
    (WmspRequestHandler)WMSP_SetBeaconPeriod,
    (WmspRequestHandler)func_arm7_027c7a8c,
    (WmspRequestHandler)WMSP_StartScanEx,
    (WmspRequestHandler)WMSP_SetWepKeyEx,
    (WmspRequestHandler)func_arm7_027c5e80,
    (WmspRequestHandler)func_arm7_027c5e4c,
    (WmspRequestHandler)func_arm7_027c5e10,
};
