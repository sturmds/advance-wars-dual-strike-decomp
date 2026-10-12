#include "sdk/wl.h"

extern void (*const gWlTaskHandlers[WIFI_TASK_SLOTS])(void);

/* Empties the task queues, gives each task its handler and queues task 12. */
void WifiTask_ClearQueues(void) {
  WlWork* work = WIFI_DRIVER_STATE;
  u32 i;

  work->topPriority = 0;
  work->runPriority = 0;
  for (i = 0; i < WIFI_TASK_LEVELS; i++) {
    work->queueHead[i] = work->queueTail[i] = WIFI_TASK_LIST_END;
  }
  for (i = 0; i < WIFI_TASK_SLOTS; i++) {
    work->tasks[i].next = WIFI_TASK_LIST_END;
    work->tasks[i].queued = 0;
    work->tasks[i].handler = gWlTaskHandlers[i];
  }
  WifiTask_Schedule(WIFI_TASK_LEVEL_IDLE, 12); /* WifiTask_WaitForHost */
}
