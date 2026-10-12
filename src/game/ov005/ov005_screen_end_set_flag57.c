#include "types.h"
#include "game/ov005_task.h"
#include "game/unlocks.h"

void Mem_Free(void* block);

/* Last step of the screen: frees its work block and sets game flag 57 (meaning not known). */
void Ov005Screen_EndSetFlag57(Ov005Task* task) {
  Mem_Free(task->work);
  GameFlag_Set(57, 1);
}
