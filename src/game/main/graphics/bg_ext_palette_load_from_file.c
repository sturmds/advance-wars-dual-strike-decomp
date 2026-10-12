#include "types.h"

void* File_Load(const char* path);
void Mem_Free(void* res);
void BgExtPalette_Load(const void* src, u32 offset, u32 size, u32 slot);

/* Loads a file and copies size bytes of it to the background extended
 * palette of the current screen: slot (8 KB each) plus offset. */
void BgExtPalette_LoadFromFile(const char* fileName, u32 offset, u32 size,
                               u32 slot) {
  void* res = File_Load(fileName);

  BgExtPalette_Load(res, offset, size, slot);
  Mem_Free(res);
}
