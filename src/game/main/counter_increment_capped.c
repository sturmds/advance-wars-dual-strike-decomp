#include "types.h"

/* 9999:59:59 in frames at 60 frames a second. */
#define FRAME_COUNTER_MAX 2159999940u

/* Adds one frame to a play time counter, stopping at FRAME_COUNTER_MAX. */
void Counter_IncrementCapped(u32* counter) {
  u32 next = *counter + 1;

  if (next >= FRAME_COUNTER_MAX) {
    *counter = FRAME_COUNTER_MAX;
  } else {
    *counter = next;
  }
}
