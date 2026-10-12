#include "types.h"
#include "game/event_queue.h"

void LinkScreen_PushBack(void) { EventQueue_Push(2, 0); }
