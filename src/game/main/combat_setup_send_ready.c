#include "types.h"
#include "game/event_queue.h"

/* Queues event 20 for the link: the Combat setup handler (CombatSetup_UpdateShop)
 * marks the sender's row ready (CombatSetup_MarkReady). */
void CombatSetup_SendReady(void) { EventQueue_Push(20, 0); }
