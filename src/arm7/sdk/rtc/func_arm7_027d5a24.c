#include "sdk/pad.h"
#include "sdk/rtc.h"
#include "sdk/os_arm7.h"

extern u16 data_arm7_027d81ac;

void func_arm7_027d5244(void);
void func_arm7_027d5364(void);
void func_arm7_027d53f4(void *arg);
void func_arm7_027d58e8(u32 tag, u32 data, BOOL err);

/* Its definition takes three arguments, but it only branches to the PXI
 * FIFO init func_arm7_037fe304, which takes none. */
void func_arm7_037fe0a0(void);
void func_arm7_037fe298(u32 tag, void (*callback)(u32 tag, u32 data, BOOL err));
void func_arm7_037fe3fc(u16 mask, u16 value);
int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fb748(u32 mask, void (*function)(void));
u32 func_arm7_037fb690(u32 mask);

/*
 * Runs once: checks the clock, starts the thread that serves the ARM9's
 * requests, and takes the RTC's alarm line on SI as the serial interrupt.
 */
void func_arm7_027d5a24(u32 priority)
{
    int irq;

    if (data_arm7_027d81ac) {
        return;
    }
    data_arm7_027d81ac = TRUE;

    data_arm7_027d81b0.busy = TRUE;
    func_arm7_027d5244();
    data_arm7_027d81b0.busy = FALSE;

    func_arm7_037fe0a0();
    func_arm7_037fe298(PXI_FIFO_TAG_RTC, func_arm7_027d58e8);
    MsgQueue_SetUpRing_Arm7(&data_arm7_027d81b0.queue, data_arm7_027d81b0.messages, 4);
    Thread_Spawn_Arm7(&data_arm7_027d81b0.thread, func_arm7_027d53f4, NULL,
                       data_arm7_027d81b0.stack + CLOCK_WORKER_STACK_LEN,
                       sizeof(data_arm7_027d81b0.stack), priority);
    Thread_MakeReady_Arm7(&data_arm7_027d81b0.thread);

    func_arm7_037fe3e0(RCNT_MODE_GPIO);
    func_arm7_037fe3fc(RCNT_SI_OUTPUT, 0);
    func_arm7_037fe3fc(RCNT_SI_IRQ, RCNT_SI_IRQ);
    irq = func_arm7_037fdb80();
    func_arm7_037fb748(IRQ_SIO, func_arm7_027d5364);
    func_arm7_037fb690(IRQ_SIO);
    func_arm7_037fdb94(irq);
}
