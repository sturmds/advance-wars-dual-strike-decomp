/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

/* Appends a byte to the output ring in the cartridge-bus buffer, unless the ring is full, and waits until the new head reads back. */
void Isd_CartRingWriteByte(IsdRing* ring, int byte) {
  u16 next;
  u16 head;
  u8* bytes;

  head = ring->head;
  next = (head + 1) & ISD_CART_RING_MASK;
  if (Isd_TestFlags(ISD_FLAG_CART_HIGH_BUFFER)) {
    bytes = (u8*)ISD_CART_BUFFER_HIGH;
  } else {
    bytes = (u8*)ISD_CART_BUFFER_LOW;
  }
  if (next != ring->tail) {
    Isd_WriteByte(bytes, head, byte);
    while (ring->head != next) {
      ring->head = next;
    }
  }
}
