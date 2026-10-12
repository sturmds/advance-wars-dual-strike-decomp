#include "types.h"
#include "sdk/os.h"

void* File_Load(const char* path);
void Mem_Free(void* ptr);
void Gfx_LoadTexPalette(const void* src, u32 offset, u32 size);

/* Loads a file, hands its first size bytes to Gfx_LoadTexPalette at
 * offset, then frees it. */
void Gfx_LoadTexPaletteFile(const char* fileName, u32 offset, u32 size) {
  void* file = File_Load(fileName);

  DC_FlushRange(file, size);
  Gfx_LoadTexPalette(file, offset, size);
  Mem_Free(file);
}
