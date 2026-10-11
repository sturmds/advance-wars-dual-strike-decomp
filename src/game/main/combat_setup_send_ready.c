#include "types.h"
#include "game/event_queue.h"

/* Queues event 20 for the link: the Combat setup handler (func_02084024)
 * marks the sender's row ready (func_020832a8). */
void CombatSetup_SendReady(void)
{
    EventQueue_Push(20, 0);
}
