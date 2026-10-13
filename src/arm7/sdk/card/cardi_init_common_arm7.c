#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"
#include "bss_run.h"

u32 data_arm7_03809260;
#pragma section BSS_RUN1 begin
CARDiSpiTransfer data_arm7_03809860;
#pragma section BSS_RUN1 end
/* aligned to 32: no symbol covers the bytes before it */
CARDiArm7Common data_arm7_03809280 __attribute__((aligned(32)));
/* 0x520 bytes no code names, before data_arm7_03809860 */
u8 data_arm7_03809340[0x520];

/* looks like OS_CreateThread */
void func_arm7_03802974(u32 state);
void Card_ServiceThread_Arm7(void* arg);
void Card_OnRequestTagMessage_Arm7(u32 tag, u32 data, int err);

#define GAMECARD_WORKER_PRIO_ARM7 4

/*
 * Clears the request and lock state, starts the card thread and listens for
 * the other CPU's requests on PXI tag 11.
 */
void CARDi_InitCommon_Arm7(void) {
  CARDiArm7Common* common = &data_arm7_03809280;

  common->lockOwner = OS_LOCK_ID_ERROR;
  common->lockCount = 0;
  common->lockTarget = 0;
  common->cmd = NULL;
  common->recvStep = 0;
  common->lockQueue = 0;
  common->queue.head = 0;
  common->priority = GAMECARD_WORKER_PRIO_ARM7;
  /* The stack ends where the SPI transfer state starts. */
  Thread_Spawn_Arm7(&common->thread, Card_ServiceThread_Arm7, NULL,
                    &data_arm7_03809860, GAMECARD_WORKER_STACK_BYTES,
                    common->priority);
  Thread_MakeReady_Arm7(&common->thread);
  Fifo_RegisterTagHandler_Arm7(FIFO_TAG_CARD_SERVICE,
                               Card_OnRequestTagMessage_Arm7);
  if (*BOOT_ORIGIN_WORD != BOOT_ORIGIN_WIRELESS) {
    func_arm7_03802974(1);
  }
}
