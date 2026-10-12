#include "types.h"
#include "game/Unk_0229c734.h"

void Mem_Free(void* block);

/* Frees the result point work area and clears the pointer. */
void ResultPoint_Free(void) {
  Mem_Free(data_0229c734);
  data_0229c734 = NULL;
}
