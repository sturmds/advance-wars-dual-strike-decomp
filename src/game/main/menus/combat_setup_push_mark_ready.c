#include "types.h"
#include "game/event_queue.h"

void CombatSetup_PushMarkReady(void)
{
    EventQueue_Push(9, 0);
}
