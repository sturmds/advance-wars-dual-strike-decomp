#include "game/event_queue.h"

void EventQueue_Reset(void)
{
    data_0229d89c.readIndex = 0;
    data_0229d89c.writeIndex = 0;
}
