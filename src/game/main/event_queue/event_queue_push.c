#include "game/event_queue.h"

/* The arg byte of each entry; the original addresses it through its own symbol. */
extern u8 data_0229d89d[];
void func_0207bd28(void);

void EventQueue_Push(int id, int arg)
{
    u8 next;
    u8 writeIndex = data_0229d89c.writeIndex;

    next = writeIndex + 1;

    if (next == EVENT_QUEUE_SIZE) {
        next = 0;
    }
    /* Full queue: the event is dropped. */
    if (next == data_0229d89c.readIndex) {
        func_0207bd28();
        return;
    }
    data_0229d89c.entries[writeIndex].id = id;
    data_0229d89d[data_0229d89c.writeIndex * 2] = arg;
    data_0229d89c.writeIndex = next;
}
