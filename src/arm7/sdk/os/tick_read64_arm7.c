#include "types.h"
#include "sdk/os_arm7.h"

extern vu64 gOsiTickCounter_Arm7;
int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/* ARM7 copy of OS_GetTick: timer 0 overflow count above its 16-bit counter. */
u64 Tick_Read64_Arm7(void)
{
    int irqState;
    vu16 counter;
    vu64 overflows;

    irqState = func_arm7_037fdb80();
    counter = REG_TM0CNT_L;
    overflows = gOsiTickCounter_Arm7 & 0x0000ffffffffffffULL;

    /* The overflow interrupt is pending but has not been counted yet. */
    if ((REG_IF & IRQ_MASK_TIMER0) && !(counter & 0x8000)) {
        overflows++;
    }
    func_arm7_037fdb94(irqState);
    return (overflows << 16) | counter;
}
