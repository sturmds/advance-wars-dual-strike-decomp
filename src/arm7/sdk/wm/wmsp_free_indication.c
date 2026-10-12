#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/wmsp.h"

/* Gives an indication from WL back to the WMSP heap it was allocated from. */
void Wmsp_FreeIndication(WlIndicationHeader* ind) {
  WmspWork* work = func_arm7_027c05e0();
  int intr = Cpu_MaskIrq_Arm7();

  Heap_ReturnBlock_Arm7(work->heapArena, work->heap, ind);
  Cpu_RestoreIrqBit_Arm7(intr);
}
