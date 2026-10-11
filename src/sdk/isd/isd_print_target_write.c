/* Needed to match: the library was built optimizing for size. */
#pragma optimize_for_size on
#include "types.h"
#include "sdk/isd.h"

void func_02120ad4(IsdRing *ring, int byte);

/*
 * Write callback of the formatter's output target: sends each byte of text to the debugger
 * through the ring chosen by the shared flags, and counts bytes stored and seen in the target.
 * Bytes past limit - 1 are counted but not sent (snprintf style).
 */
void Isd_PrintTargetWrite(IsdPrintTarget *target, const char *text, u32 length)
{
    volatile IsdSharedBlock *shared = ISD_SHARED;
    IsdRing *ring = (IsdRing *)(shared->channel + ISD_RING_OFFSET);
    IsdByteWriter writer = NULL;
    IsdBusState busState;
    u32 savedState;

    if (length != 0 && Isd_TestFlags(ISD_FLAG_OUTPUT)) {
        if (Isd_TestFlags(ISD_FLAG_CART_RING)) {
            writer = Isd_CartRingWriteByte;
        } else if (Isd_TestFlags(ISD_FLAG_CHANNEL_RING)) {
            writer = func_02120ad4;
        }
        if (writer != NULL) {
            Isd_AcquireCartridge(&savedState);
            Isd_BusSave(&busState);
            if (target != NULL) {
                while (length-- != 0) {
                    if (target->stored < target->limit - 1) {
                        u16 byte = Isd_ReadByte(text);
                        writer(ring, byte & 0xff);
                        target->stored++;
                    }
                    target->total++;
                    text++;
                }
            }
            Isd_BusRestore(&busState);
            Isd_ReleaseCartridge(savedState);
        }
    }
}
