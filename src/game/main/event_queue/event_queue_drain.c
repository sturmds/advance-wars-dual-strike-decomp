#include "game/event_queue.h"

extern u8 gEventQueueBatch[];

void func_0207b4b0(void *events);

/* Moves up to 4 events into gEventQueueBatch (id, arg pairs), pads the rest
 * with id 0 and hands them on. Needed to match: writeIndex is read in the loop
 * condition, not kept in a local. */
void EventQueue_Drain(void)
{
    int count = 0;

    while (gEventQueue.readIndex != gEventQueue.writeIndex) {
        u8 readIndex = gEventQueue.readIndex;
        gEventQueueBatch[count * 2] = gEventQueue.entries[readIndex].id;
        gEventQueueBatch[count * 2 + 1] = gEventQueue.entries[readIndex].arg;
        gEventQueue.readIndex++;
        if (gEventQueue.readIndex == EVENT_QUEUE_SIZE) {
            gEventQueue.readIndex = 0;
        }
        count++;
        if (count == 4) {
            break;
        }
    }
    while (count < 4) {
        gEventQueueBatch[count * 2] = 0;
        count++;
    }
    func_0207b4b0(gEventQueueBatch);
}
