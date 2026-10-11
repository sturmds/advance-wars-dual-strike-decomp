#include "types.h"
#include "game/event_queue.h"

/* Queues link message 5: move this console's player down one panel. */
void LinkScreen_PushMoveDown(void)
{
    EventQueue_Push(5, 0);
}
