#include "types.h"
#include "sdk/os.h"

void* File_Load(const char* path);
void GfxQueue_AddKind4(const void* src, void* dst, u32 size);

/* Loads a file and queues its first size bytes as a kind 4 copy to dest
 * (callers pass a 32-byte palette file). The queue entry keeps the loaded
 * file, which is not freed here. */
void Gfx_QueueFileCopy(const char* fileName, void* dest, int size) {
  void* file = File_Load(fileName);

  DC_FlushRange(file, size);
  GfxQueue_AddKind4(file, dest, size);
}
