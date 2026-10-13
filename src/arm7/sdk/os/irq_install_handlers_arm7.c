#include "sdk/os_arm7.h"
#include "sdk/os_common.h"
#include "bss_run.h"

extern OSIrqFunction data_arm7_03807750
    []; /* handlers for the interrupts that have no slot, indexed by IE bit */
#pragma section BSS_RUN1 begin
BOOL gThreadTableReady_Arm7;
u32 gSchedulerHoldDepth_Arm7;
#pragma section BSS_RUN1 end
#pragma section BSS_RUN2 begin
BOOL data_arm7_03807a50; /* set once the arenas are set up */
#pragma section BSS_RUN2 end
#pragma section BSS_RUN1 begin
BOOL data_arm7_038078f8; /* set once the locks are set up */
OSThread** gRunningThreadRef_Arm7;
/* called on every switch, before info->switchCallback */
ThreadSwapHook data_arm7_03807908;
#pragma section BSS_RUN1 end
/* data_arm7_0380788c[OS7_IRQ_SLOT_VBLANK] */
static IrqSlotEntry data_arm7_038078ec;
#pragma section BSS_RUN2 begin
/* heap info of each arena, by arena id; size: up to the next symbol */
OSHeapInfo* data_arm7_03807a54[0x9];
#pragma section BSS_RUN2 end
#pragma section BSS_RUN1 begin
OSThreadInfo data_arm7_0380790c;
OSThread gBootThread_Arm7; /* the code that called this, made thread 0 */
/* where ARM9 checks for its idle thread; no ARM7 code seen creates it */
OSThread data_arm7_03807960;
#pragma section BSS_RUN1 end

/* ARM7 copy of OS_SetIrqFunction. Unlike ARM9, VBlank (bit 0) has a slot too. */
void Irq_InstallHandlers_Arm7(OSIrqMask mask, OSIrqFunction function) {
  int bit;

  for (bit = 0; bit < 25; mask >>= 1, bit++) {
    IrqSlotEntry* slot;

    if ((mask & 1) == 0) {
      continue;
    }
    slot = NULL;
    if (bit >= 8 && bit <= 11) {
      slot = &data_arm7_0380788c[bit - 8];
    } else if (bit >= 3 && bit <= 6) {
      slot = &data_arm7_0380788c[bit + 1];
    } else if (bit == 0) {
      slot = &data_arm7_038078ec;
    } else {
      data_arm7_03807750[bit] = function;
    }
    if (slot != NULL) {
      /* slot handlers take an argument, which is NULL here */
      slot->handler = (void (*)(void*))function;
      slot->arg = NULL;
      slot->keepEnabled = TRUE;
    }
  }
}
