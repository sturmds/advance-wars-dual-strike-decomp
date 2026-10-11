#include "types.h"

BOOL EventScript_IsRunning(void);
void EventScript_StartQueued(void);
void *EventScript_ReleaseInput(void);

/* Clears the request flag. If it was set and no script runs, resets
 * gEventScriptOwnsInput (EventScript_ReleaseInput) and starts the next queued script. */
void EventScript_StartQueuedIfRequested(int *startRequested)
{
    if (*startRequested != 0) {
        *startRequested = 0;
        if (EventScript_IsRunning()) {
            return;
        }
        EventScript_ReleaseInput();
        EventScript_StartQueued();
    } else {
        *startRequested = 0; /* needed to match: the clear is written in both branches */
    }
}
