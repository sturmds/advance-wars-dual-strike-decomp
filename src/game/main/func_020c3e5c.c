#include "types.h"
#include "game/map_data.h"

void Mem_Free(void* block);
extern MapData* data_0229eadc;

void func_020c3e5c(void) { Mem_Free(data_0229eadc); }
