#include "types.h"
#include "game/Unk_020226d4.h"

void Mem_Free(void* handle);

void func_020d4278(Unk_020226d4* arg) { Mem_Free(arg->unk_50); }
