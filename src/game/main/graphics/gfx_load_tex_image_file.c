#include "types.h"

void *File_Load(const char *path);
void *Mem_Alloc(u32 size);
void Mem_Free(void *ptr);
void Data_Unpack(const void *src, void *dst);
void DC_FlushRange(void *data, u32 size);
void Gfx_LoadTexImage(const void *src, u32 offset, u32 size);  /* guess: copies to texture image VRAM, twin of Gfx_LoadTexPalette */

/* Loads a compressed file and copies the unpacked image to offset in texture
 * image VRAM. The unpacked size is in the top 24 bits of the first word. */
void Gfx_LoadTexImageFile(const char *fileName, u32 offset)
{
    void *file = File_Load(fileName);
    u32 size = *(u32 *)file >> 8;
    void *image = Mem_Alloc(size);

    Data_Unpack(file, image);
    DC_FlushRange(image, size);
    Gfx_LoadTexImage(image, offset, size);
    Mem_Free(image);
    Mem_Free(file);
}
