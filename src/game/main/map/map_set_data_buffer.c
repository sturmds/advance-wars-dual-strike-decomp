#include "types.h"
#include "game/map_data.h"

extern MapData* data_0229eadc;

/* Remembers the buffer that Map_LoadData fills with map data. */
void Map_SetDataBuffer(void* buffer) { data_0229eadc = buffer; }
