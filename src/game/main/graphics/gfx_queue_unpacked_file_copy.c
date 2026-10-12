#include "types.h"
#include "sdk/os.h"

void *File_Load(const char *path);
void *Mem_Alloc(s32 size);
void Mem_Free(void *ptr);
void Data_Unpack(const void *src, void *dst);
void GfxQueue_AddKind3(const void *src, void *dst, u32 size);

/* Loads a compressed file, unpacks it (the top 24 bits of its first word
 * are the unpacked size) and queues the result as a kind 3 copy to dest.
 * The packed file is freed; the unpacked buffer stays for the queue. */
void Gfx_QueueUnpackedFileCopy(const char *fileName, void *dest)
{
    u8 *packed = File_Load(fileName);
    u32 size = *(u32 *)packed >> 8;
    void *unpacked = Mem_Alloc(size);

    Data_Unpack(packed, unpacked);
    DC_FlushRange(unpacked, size);
    GfxQueue_AddKind3(unpacked, dest, size);
    Mem_Free(packed);
}
