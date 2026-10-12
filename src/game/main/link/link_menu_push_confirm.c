#include "types.h"
#include "game/event_queue.h"

void LinkMenu_PushConfirm(void) { EventQueue_Push(4, 0); }
