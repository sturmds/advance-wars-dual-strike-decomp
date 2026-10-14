#include "types.h"
#include "game/link_buffers.h"

void Mem_CopyBytes(int, void*, int);

void func_0207b4b0(int a0) {
  Mem_CopyBytes(a0, data_0229ca48, LINK_SHARED_BLOCK_SIZE);
}
