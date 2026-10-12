#include "game/event_queue.h"

void EventQueue_Reset(void) {
  gEventQueue.readIndex = 0;
  gEventQueue.writeIndex = 0;
}
