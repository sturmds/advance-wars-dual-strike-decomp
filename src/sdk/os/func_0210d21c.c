#include "types.h"
#include "sdk/os.h"

typedef struct Unk_0210d21c Unk_0210d21c;

/* Handshake block at the top of main memory, shared with the ARM7. */
struct Unk_0210d21c {
  u32 unk_0;
  u16 unk_4;
  u16 unk_6;
};

#define SHARED_BLOCK ((Unk_0210d21c*)0x027ffff0)

static u32 data_022a991c;

void OSi_WaitByLoop(void);
void OS_UnlockByWord(int tag, void* block, int arg);
void OS_LockByWord(int tag, void* block, int arg);
void MIi_CpuClear32(u32 value, void* dest, u32 size);

void func_0210d21c(void) {
  Unk_0210d21c* block;
  vu32 zero;

  if (data_022a991c != 0) {
    return;
  }
  data_022a991c = 1;
  SHARED_BLOCK->unk_0 = 0;
  OS_LockByWord(0x7e, SHARED_BLOCK, 0);
  block = SHARED_BLOCK;
  while (block->unk_6 != 0) {
    OSi_WaitByLoop();
  }
  *(vu32*)0x027fffb0 = -1;
  *(vu32*)0x027fffb4 = -0x10000;
  zero = 0;
  MIi_CpuClear32(zero, (void*)0x027fffc0, 0x28);
  REG_EXMEMCNT |= 0x800;
  REG_EXMEMCNT |= 0x80;
  OS_UnlockByWord(0x7e, SHARED_BLOCK, 0);
  OS_LockByWord(0x7f, SHARED_BLOCK, 0);
}
