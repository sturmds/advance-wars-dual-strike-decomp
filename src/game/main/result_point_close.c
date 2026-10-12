#include "types.h"
#include "game/task.h"
#include "game/Unk_0229c734.h"

void Task_DestroyByScript(const void* script);
void Mem_Free(void* block);

extern const TaskScriptCmd data_0215a44c[];

/* Ends the result point task and frees its work area. */
void ResultPoint_Close(void) {
  Task_DestroyByScript(data_0215a44c);
  if (data_0229c734 != NULL) {
    Mem_Free(data_0229c734);
  }
}
