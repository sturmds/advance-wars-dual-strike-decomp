#include "types.h"
#include "game/event_queue.h"

/* Queues link message 1: this console is ready (see LinkPhase_Update). */
void LinkScreen_PushReady(void) { EventQueue_Push(1, 0); }
