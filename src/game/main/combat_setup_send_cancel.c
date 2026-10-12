#include "types.h"
#include "game/event_queue.h"

/* Queues event 24 for the link: CombatSetup_UpdateShop takes back the sender's
 * ready mark, or else sells its last unit, or leaves when the row is empty. */
void CombatSetup_SendCancel(void) { EventQueue_Push(24, 0); }
