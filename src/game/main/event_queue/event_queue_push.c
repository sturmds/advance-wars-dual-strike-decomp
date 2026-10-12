#include "game/event_queue.h"

/* The arg byte of each entry; the original addresses it through its own symbol. */
extern u8 data_0229d89d[];
void Link_FlagError(void);

void EventQueue_Push(int id, int arg) {
  u8 next;
  u8 writeIndex = gEventQueue.writeIndex;

  next = writeIndex + 1;

  if (next == EVENT_QUEUE_SIZE) {
    next = 0;
  }
  /* Full queue: the event is dropped. */
  if (next == gEventQueue.readIndex) {
    Link_FlagError();
    return;
  }
  gEventQueue.entries[writeIndex].id = id;
  data_0229d89d[gEventQueue.writeIndex * 2] = arg;
  gEventQueue.writeIndex = next;
}
