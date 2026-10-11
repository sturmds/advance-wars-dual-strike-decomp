#include "types.h"
#include "game/event_queue.h"

void LinkMenu_PushCancel(void)
{
    EventQueue_Push(5, 0);
}
