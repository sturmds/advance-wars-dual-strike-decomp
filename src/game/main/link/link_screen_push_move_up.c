#include "types.h"
#include "game/event_queue.h"

/* Queues link message 4: move this console's player up one panel. */
void LinkScreen_PushMoveUp(void) { EventQueue_Push(4, 0); }
