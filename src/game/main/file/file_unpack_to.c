#include "types.h"
#include "sdk/fs.h"
#include "sdk/gx.h"
#include "sdk/os.h"
#include "game/heap.h"

void Unpack_Lz77ByHalfwords(const void *src, void *dest);             /* SDK LZ77 unpack */
void Unpack_HuffmanToWords(const void *src, void *dest);             /* SDK Huffman unpack */
void Unpack_RunLengthByHalfwords(const void *src, void *dest);             /* SDK run-length unpack */

/* DS compression header, first word of a packed file: type in bits
 * 4-7, unpacked size in bits 8-31. */
#define COMPRESS_LZ77    0x10
#define COMPRESS_HUFFMAN 0x20
#define COMPRESS_RL      0x30

/* Reads a compressed file into a temporary heap block and unpacks it to
 * dest. Returns the unpacked size rounded up to 16 bytes. If the file
 * cannot be opened, allocated or read, turns the main screen bright and
 * halts. */
u32 File_UnpackTo(void *dest, const char *path)
{
    FSFile file;
    s32 size;
    u32 *packed;
    u32 unpackedSize;

    FS_InitFile(&file);
    if (FS_OpenFile(&file, path)) {
        size = file.bottom - file.top;
        packed = Heap_Alloc(gGameHeap, size, -16);
        if (packed == NULL) {
            FS_CloseFile(&file);
            GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
            OS_Terminate();
            return 0;
        }
        if (size != FS_ReadFile(&file, packed, size)) {
            Heap_Free(gGameHeap, packed);
            FS_CloseFile(&file);
            GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
            OS_Terminate();
            return 0;
        }
        FS_CloseFile(&file);
    } else {
        GXx_SetMasterBrightness_(REG_MASTER_BRIGHT, 12);
        OS_Terminate();
        return 0;
    }

    unpackedSize = ((*packed >> 8) + 15) & ~15;
    switch (*packed & 0xf0) {
    case COMPRESS_LZ77:
        Unpack_Lz77ByHalfwords(packed, dest);
        break;
    case COMPRESS_HUFFMAN:
        Unpack_HuffmanToWords(packed, dest);
        break;
    case COMPRESS_RL:
        Unpack_RunLengthByHalfwords(packed, dest);
        break;
    }
    Heap_Free(gGameHeap, packed);
    return unpackedSize;
}
