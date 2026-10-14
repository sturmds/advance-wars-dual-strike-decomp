#include "game/event_queue.h"
#include "game/link_menu_state.h"

static u8 gEventQueueBatch[2 * EVENT_BATCH_SIZE];
LinkMenuState data_0229d88c;
EventQueue gEventQueue;

void func_0207b4b0(void* events);

/* Moves up to EVENT_BATCH_SIZE events into gEventQueueBatch (id, arg pairs), pads the rest
 * with id 0 and hands them on. Needed to match: writeIndex is read in the loop
 * condition, not kept in a local. */
void EventQueue_Drain(void) {
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
    if (count == EVENT_BATCH_SIZE) {
      break;
    }
  }
  while (count < EVENT_BATCH_SIZE) {
    gEventQueueBatch[count * 2] = 0;
    count++;
  }
  func_0207b4b0(gEventQueueBatch);
}
