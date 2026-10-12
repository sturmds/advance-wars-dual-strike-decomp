#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/mi.h"

/*
 * Sets up the allocator in the ARM7 private WRAM arena with room for one
 * heap, clears the rest of the arena, makes one heap over it and selects
 * it. Halts if the heap cannot be made. Returns the heap handle.
 */
s32 InitWramHeap(void) {
  void* hi;
  void* lo;
  s32 heap;

  hi = Arena_HighEnd_Arm7(OS_ARENA_WRAM_SUBPRIV);
  lo = Heap_SetUpArenaTable_Arm7(
      OS_ARENA_WRAM_SUBPRIV, Arena_LowEnd_Arm7(OS_ARENA_WRAM_SUBPRIV), hi, 1);
  Mem_FillBytes_Arm7(lo, 0,
                     (u32)Arena_HighEnd_Arm7(OS_ARENA_WRAM_SUBPRIV) - (u32)lo);
  Arena_SetLowEnd_Arm7(OS_ARENA_WRAM_SUBPRIV, lo);
  hi = Arena_HighEnd_Arm7(OS_ARENA_WRAM_SUBPRIV);
  heap = Heap_CarveFromRange_Arm7(OS_ARENA_WRAM_SUBPRIV,
                                  Arena_LowEnd_Arm7(OS_ARENA_WRAM_SUBPRIV), hi);
  if (heap < 0) {
    Cpu_HaltForever_Arm7();
  }
  Heap_SelectDefault_Arm7(OS_ARENA_WRAM_SUBPRIV, heap);
  Heap_VerifyAndCountFree_Arm7(OS_ARENA_WRAM_SUBPRIV, heap);
  return heap;
}
