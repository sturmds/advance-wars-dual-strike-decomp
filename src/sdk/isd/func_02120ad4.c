/* WIP, 89%: only the registers holding the channel pointer and the ring tail are swapped. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

/* Appends a byte to the output ring in the debugger's channel, unless the ring is full. */
void func_02120ad4(IsdRing *ring, int byte)
{
    u16 next;
    u16 head;
    u8 *bytes;
    volatile IsdSharedBlock *shared = ISD_SHARED;

    head = ring->head;
    next = (head + 1) & ISD_RING_MASK;
    bytes = shared->channel + ISD_RING_BYTES_OFFSET;
    if (next != ring->tail) {
        Isd_WriteByte(bytes, head, byte);
        ring->head = next;
    }
}
