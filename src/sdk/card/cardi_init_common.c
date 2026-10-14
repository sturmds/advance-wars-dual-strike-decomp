#pragma opt_propagation off
#include "sdk/card.h"
#include "sdk/os.h"
#include "sdk/rom_header.h"

/* aligned to 32: no symbol covers the bytes before it */
static __attribute__((aligned(32))) CARDiCommandArg data_022ab960;
static RomHeader data_022ab9a0;
CARDiCommon data_022abb00;

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

/* Needed to match: the first four stores go through data_022abb00, not
 * common, and the zero passed to MIi_CpuClearFast goes through a volatile
 * stack slot. */
void CARDi_InitCommon(void) {
  CARDiCommon* common = &data_022abb00;
  vu32 zero;
  u32 value;

  data_022abb00.lockOwner = OS_LOCK_ID_ERROR;
  data_022abb00.lockCount = 0;
  zero = 0;
  value = zero;
  data_022abb00.lockTarget = 0;
  data_022abb00.cmd = &data_022ab960;
  MIi_CpuClearFast(value, &data_022ab960, sizeof(CARDiCommandArg));
  DC_FlushRange(&data_022ab960, sizeof(CARDiCommandArg));

  Mem_CopyBytes(*BOOT_ORIGIN_WORD == BOOT_ORIGIN_WIRELESS
                    ? ROM_HEADER_BUF_DOWNLOAD_PLAY
                    : ROM_HEADER_BUF,
                &data_022ab9a0, sizeof(RomHeader));
  common->lockQueue.head = 0;
  common->queue.head = 0;
  common->priorityMode = 4;
  /* The original passes the end of common->stack as the address of the
   * variable that follows it. */
  OS_CreateThread(&common->thread, CARDi_TaskThread, NULL, &data_022ac0e0,
                  GAMECARD_WORKER_STACK_BYTES, common->priorityMode);
  OS_WakeupThreadDirect(&common->thread);
  PXI_SetFifoRecvCallback(11, Card_OnRequestTagMessage);
  if (*BOOT_ORIGIN_WORD != BOOT_ORIGIN_WIRELESS) {
    func_0211549c(1);
  }
}
