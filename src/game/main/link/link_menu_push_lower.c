#include "types.h"
#include "game/event_queue.h"

void LinkMenu_PushLower(void) { EventQueue_Push(8, 0); }
