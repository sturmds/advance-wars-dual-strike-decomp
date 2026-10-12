#include "types.h"
#include "game/ov005_task.h"
#include "game/Unk_ov005_0236bb80.h"

void DmaTasks_Destroy(void);
void Mem_Free(void* block);

/* Stops the DMA tasks that feed the line buffers, then frees the buffers. The work block stays. */
void Ov005Ripple_FreeBuffers(Ov005Task* task) {
  Unk_ov005_0236bb80* work = task->work;

  DmaTasks_Destroy();
  Mem_Free(work->yBufferB);
  Mem_Free(work->xBufferB);
  Mem_Free(work->yBufferA);
  Mem_Free(work->xBufferA);
  Mem_Free(work->yOffsets);
  Mem_Free(work->xOffsets);
}
