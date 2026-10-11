#include "game/event_queue.h"

extern u8 data_0229d894[];

void func_0207b4b0(void *events);

/* Moves up to 4 events into data_0229d894 (id, arg pairs), pads the rest with id 0 and hands them on. */
void EventQueue_Drain(void)
{
    int count = 0;
    u8 writeIndex = gEventQueue.writeIndex;
    u8 readIndex = gEventQueue.readIndex;

    while (readIndex != writeIndex) {
        data_0229d894[count * 2] = gEventQueue.entries[readIndex].id;
        data_0229d894[count * 2 + 1] = gEventQueue.entries[readIndex].arg;
        gEventQueue.readIndex++;
        if (gEventQueue.readIndex == EVENT_QUEUE_SIZE) {
            gEventQueue.readIndex = 0;
        }
        count++;
        if (count == 4) {
            break;
        }
        readIndex = gEventQueue.readIndex;
    }
    while (count < 4) {
        data_0229d894[count * 2] = 0;
        count++;
    }
    func_0207b4b0(data_0229d894);
}
