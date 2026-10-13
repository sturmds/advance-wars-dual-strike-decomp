#include "sdk/os_arm7.h"

/* 0x1c bytes no code names, before data_arm7_038078bc */
u8 data_arm7_038078a0[0x1c];
/* The timer entries of the ARM7 IRQ slot table, from data_arm7_0380788c[4], reached through three symbols. */
IrqSlotEntry data_arm7_038078bc[0x4]; /* size: up to the next symbol */
extern IrqSlotKeepAlias data_arm7_038078c0[];
extern IrqSlotArgAlias data_arm7_038078c4[];

/* ARM7 copy of Timer_InstallIrqSlot: sets the handler and its argument for a timer interrupt, then enables it. */
void Timer_InstallIrqSlot_Arm7(int timer, void (*handler)(void*), void* arg) {
  data_arm7_038078bc[timer].handler = handler;
  data_arm7_038078c4[timer].arg = arg;
  Irq_SetEnableBits_Arm7(1 << (timer + 3));
  data_arm7_038078c0[timer].keepEnabled = TRUE;
}
