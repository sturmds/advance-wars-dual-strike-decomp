#include "types.h"
#include "game/page_strip.h"

void Mem_Free(void* block);

extern PageStrip* data_0229c744;

void func_02046fc8(void) { Mem_Free(data_0229c744); }
