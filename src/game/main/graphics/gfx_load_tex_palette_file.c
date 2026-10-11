#include "types.h"

void *File_Load(const char *path);
void Mem_Free(void *ptr);
void func_0210e138(void *data, u32 size);
void Gfx_LoadTexPalette(const void *src, u32 offset, u32 size);

/* Loads a file, hands its first size bytes to Gfx_LoadTexPalette at
 * offset, then frees it. */
void Gfx_LoadTexPaletteFile(const char *fileName, u32 offset, u32 size)
{
    void *file = File_Load(fileName);

    func_0210e138(file, size);
    Gfx_LoadTexPalette(file, offset, size);
    Mem_Free(file);
}
