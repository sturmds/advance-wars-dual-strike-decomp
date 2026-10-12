#include "types.h"
#include "game/event_queue.h"

void LinkMenu_PushRaise(void) { EventQueue_Push(7, 0); }
