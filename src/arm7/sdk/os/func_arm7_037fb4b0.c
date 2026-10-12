#include "sdk/os.h"
#include "sdk/pm.h"

/* VBlank interrupt handler, entry 0 of the ARM7 IRQ table: counts the frame and runs the VBlank slot's handler. */
void func_arm7_037fb4b0(void)
{
    IrqSlotEntry *slots = OS7_IRQ_SLOTS;   /* needed to match: keeps the base and the 0x60 offset apart */
    void (*handler)(void) = (void (*)(void))slots[OS7_IRQ_SLOT_VBLANK].handler;

    (*SHARED_FRAME_COUNTER)++;
    if (handler != NULL) {
        handler();
    }
    OS7_IRQ_CHECK_FLAGS |= 1;
}
