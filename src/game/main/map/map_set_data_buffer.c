#include "types.h"

extern void* data_0229eadc;

/* Remembers the buffer that Map_LoadData fills with map data. */
void Map_SetDataBuffer(void* buffer) { data_0229eadc = buffer; }
