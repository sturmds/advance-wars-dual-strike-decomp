#include "types.h"

void *File_Load(const char *path);
void func_0210e138(void *data, u32 size);
void GfxQueue_AddKind4(u32 a, u32 b, u32 size);

/* Loads a file and queues its first size bytes as a kind 4 copy to dest
 * (callers pass a 32-byte palette file). The queue entry keeps the loaded
 * file, which is not freed here. */
void Gfx_QueueFileCopy(const char *fileName, void *dest, int size)
{
    void *file = File_Load(fileName);

    func_0210e138(file, size);
    GfxQueue_AddKind4((u32)file, (u32)dest, size);
}
