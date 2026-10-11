#include "types.h"
#include "game/event_queue.h"

/* Queues link message 3: move this console's player to the given panel. */
void LinkScreen_PushMoveToPanel(int a0)
{
    EventQueue_Push(3, a0);
}
