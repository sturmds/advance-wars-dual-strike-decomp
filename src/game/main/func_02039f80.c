#include "types.h"

void* ResultObj_Load(int firstTile, int palette);
void Mem_Free(void* block);

void func_02039f80(int firstTile, int palette) {
  void* t0 = ResultObj_Load(firstTile, palette);
  Mem_Free(t0);
}
