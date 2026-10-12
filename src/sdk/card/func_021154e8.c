#pragma opt_propagation off
#include "sdk/card.h"
#include "sdk/os.h"

extern CARDiCommandArg data_022ab960;
extern u8 data_022ab9a0[0x160];
extern u8 data_022ac0e0[];

void MIi_CpuClearFast(u32 value, void* dst, u32 size);
void Mem_CopyBytes(const void* src, void* dst, u32 size);
void OS_CreateThread(OSThread* thread, void (*proc)(void*), void* arg,
                     void* stack, u32 stackSize, u32 priority);
void OS_WakeupThreadDirect(OSThread* thread);
void PXI_SetFifoRecvCallback(int tag,
                             void (*callback)(int tag, u32 data, int err));
void func_0211549c(u32 state);
void CARDi_TaskThread(void* arg);
void Card_OnRequestTagMessage(int tag, u32 data, int err);

/* Not matched yet (96.8%): the original schedules the zero's mov before the
 * lockOwner store. Writing lockTarget and cmd through data_022abb00 rather
 * than common got it this far. */
void func_021154e8(void) {
  CARDiCommon* common = &data_022abb00;
  vu32 zero;
  u32 value;

  common->lockOwner = OS_LOCK_ID_ERROR;
  common->lockCount = 0;
  zero = 0;
  value = zero;
  data_022abb00.lockTarget = 0;
  data_022abb00.cmd = &data_022ab960;
  MIi_CpuClearFast(value, &data_022ab960, sizeof(CARDiCommandArg));
  DC_FlushRange(&data_022ab960, sizeof(CARDiCommandArg));

  Mem_CopyBytes(*BOOT_ORIGIN_WORD == BOOT_ORIGIN_WIRELESS ? (void*)0x023fe940
                                                          : (void*)0x027ffe00,
                data_022ab9a0, 0x160);
  common->lockQueue.head = 0;
  common->queue.head = 0;
  common->priorityMode = 4;
  OS_CreateThread(&common->thread, CARDi_TaskThread, NULL, data_022ac0e0,
                  GAMECARD_WORKER_STACK_BYTES, common->priorityMode);
  OS_WakeupThreadDirect(&common->thread);
  PXI_SetFifoRecvCallback(11, Card_OnRequestTagMessage);
  if (*BOOT_ORIGIN_WORD != BOOT_ORIGIN_WIRELESS) {
    func_0211549c(1);
  }
}
