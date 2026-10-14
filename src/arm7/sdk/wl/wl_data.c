#include "types.h"
#include "sdk/wl.h"

void WifiMlme_RunScan(void);
void func_arm7_027cc5ac(void);
void WifiMlme_RunAuthentication(void);
void WifiMlme_RunAssociation(void);
void WifiMlme_RunReassociation(void);
void WifiMlme_RunChannelSurvey(void);
void WifiTask_DeliverData(void);
void WifiTask_RouteMgmtFrame(void);
void WifiTask_RefreshBeacon(void);
void WifiTask_SortDataFrame(void);
void WifiTask_AgeStations(void);
void WifiTask_DispatchCommand(void);
void func_arm7_027cbfa4(void);
void WifiTask_CollectTxResults(void);
void WifiTask_DrainRxRing(void);
void WifiTask_FinishMpPoll(void);
void WifiTask_AgeReassembly(void);
void WifiTask_AgeNetworks(void);
void WifiTask_ResumeSending(void);
void WifiTask_ReportEvents(void);
void WifiTask_BeginShutdown(void);
void WifiTask_Shutdown(void);

/* declared `void (*const gWlTaskHandlers[WIFI_TASK_SLOTS])(void)` in wifi_task_clear_queues.c: cannot read the declaration `void (*const gWlTaskHandlers[WIFI_TASK_SLOTS])(void)` */
void* gWlTaskHandlers[] = {
    &WifiMlme_RunScan,           &func_arm7_027cc5ac,
    &WifiMlme_RunAuthentication, &WifiMlme_RunAssociation,
    &WifiMlme_RunReassociation,  &WifiMlme_RunChannelSurvey,
    &WifiTask_DeliverData,       &WifiTask_RouteMgmtFrame,
    &WifiTask_RefreshBeacon,     &WifiTask_SortDataFrame,
    &WifiTask_AgeStations,       &WifiTask_DispatchCommand,
    &WifiTask_WaitForHost,       &func_arm7_027cbfa4,
    &WifiTask_CollectTxResults,  &WifiTask_DrainRxRing,
    &WifiTask_FinishMpPoll,      &WifiTask_AgeReassembly,
    &WifiTask_AgeNetworks,       &WifiTask_DrainOutbox,
    &WifiTask_ResumeSending,     &WifiTask_ReportEvents,
    &WifiTask_BeginShutdown,     &WifiTask_Shutdown};
