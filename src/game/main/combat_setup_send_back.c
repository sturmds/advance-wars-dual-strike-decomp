#include "types.h"
#include "game/event_queue.h"

/* Queues event 21 for the link: the Combat setup handler (func_02084024)
 * takes back the sender's ready mark, or leaves the screen when there is none. */
void CombatSetup_SendBack(void)
{
    EventQueue_Push(21, 0);
}
