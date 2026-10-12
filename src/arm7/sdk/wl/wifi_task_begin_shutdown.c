#include "sdk/wl.h"

void func_arm7_037f92c8(void); /* stops the current session */
void func_arm7_027c9438(void);

/*
 * Task 22: sets status bit 0x8000, stops what is running, drops the tasks
 * queued at the lowest priority and queues task 23 (WifiTask_Shutdown).
 */
void WifiTask_BeginShutdown(void)
{
    WIFI_DRIVER_STATE->state.unk_12 |= 0x8000;
    if (WIFI_DRIVER_STATE->state.mode != 0) {
        func_arm7_037f92c8();
        if (WIFI_JOB_AREA_OF(WIFI_DRIVER_STATE)->unk_00 != 0) {
            WIFI_JOB_AREA_OF(WIFI_DRIVER_STATE)->unk_00 = 0;
            WIFI_JOB_AREA_OF(WIFI_DRIVER_STATE)->unk_1c->unk_04 = 6;
            WifiHost_ReturnCurrentRequest();
        }
        func_arm7_027c9438();
    }
    while (WifiTask_PopNext(WIFI_TASK_LEVEL_IDLE) != WIFI_TASK_LIST_END) {
    }
    WifiTask_Schedule(WIFI_TASK_LEVEL_IDLE, 23);
}
