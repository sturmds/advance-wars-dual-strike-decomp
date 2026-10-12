/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

/* Appends a byte to the output ring in the debugger's channel, unless the ring is full. */
void Isd_ChannelRingWriteByte(IsdRing* ring, int byte) {
  u16 next;
  u16 head;
  u8* bytes;
  volatile IsdSharedBlock* shared = ISD_SHARED;

  /* Needed to match: this dead read of the tail moves the channel pointer
     * and the tail into the original's registers (found by decomp-permuter). */
  (void)(ring->tail && ring->tail);

  head = ring->head;
  next = (head + 1) & ISD_RING_MASK;
  bytes = shared->channel + ISD_RING_BYTES_OFFSET;
  if (next != ring->tail) {
    Isd_WriteByte(bytes, head, byte);
    ring->head = next;
  }
}
