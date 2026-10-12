#include "types.h"
#include "game/event_queue.h"

void LinkMenu_PushSelectEntry(int a0) { EventQueue_Push(6, a0); }
