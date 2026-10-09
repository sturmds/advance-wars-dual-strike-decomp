#include "types.h"
#include "sdk/fs.h"

void func_02112bb4(FSFile *file);                            /* FS_InitFile */
BOOL func_02112864(FSFile *file, const char *path);          /* FS_OpenFile */
s32 func_02112718(FSFile *file, void *dest, s32 size);       /* FS_ReadFile */
BOOL func_0211281c(FSFile *file);                            /* FS_CloseFile */
void *func_021044d4(void *heap, s32 size, int align);        /* heap alloc; negative align takes the top end */
void func_02104494(void *heap, void *block);                 /* heap free */
void func_0210f78c(const void *src, void *dest);             /* SDK LZ77 unpack */
void func_0210f864(const void *src, void *dest);             /* SDK Huffman unpack */
void func_0210f9a8(const void *src, void *dest);             /* SDK run-length unpack */
void func_0210a424(volatile u16 *reg, int brightness);       /* sets a master brightness register */
void func_0210eb88(void);                                    /* halts for good */

extern void *gGameHeap;                                  /* game heap */

#define REG_MASTER_BRIGHT ((volatile u16 *)0x0400006c)

/* DS compression header, first word of a packed file: type in bits
 * 4-7, unpacked size in bits 8-31. */
#define COMPRESS_LZ77    0x10
#define COMPRESS_HUFFMAN 0x20
#define COMPRESS_RL      0x30

/* Reads a compressed file into a temporary heap block and unpacks it to
 * dest. Returns the unpacked size rounded up to 16 bytes. If the file
 * cannot be opened, allocated or read, turns the main screen bright and
 * halts. */
u32 File_LoadTo(void *dest, const char *path)
{
    FSFile file;
    s32 size;
    u32 *packed;
    u32 unpackedSize;

    func_02112bb4(&file);
    if (func_02112864(&file, path)) {
        size = file.bottom - file.top;
        packed = func_021044d4(gGameHeap, size, -16);
        if (packed == NULL) {
            func_0211281c(&file);
            func_0210a424(REG_MASTER_BRIGHT, 12);
            func_0210eb88();
            return 0;
        }
        if (size != func_02112718(&file, packed, size)) {
            func_02104494(gGameHeap, packed);
            func_0211281c(&file);
            func_0210a424(REG_MASTER_BRIGHT, 12);
            func_0210eb88();
            return 0;
        }
        func_0211281c(&file);
    } else {
        func_0210a424(REG_MASTER_BRIGHT, 12);
        func_0210eb88();
        return 0;
    }

    unpackedSize = ((*packed >> 8) + 15) & ~15;
    switch (*packed & 0xf0) {
    case COMPRESS_LZ77:
        func_0210f78c(packed, dest);
        break;
    case COMPRESS_HUFFMAN:
        func_0210f864(packed, dest);
        break;
    case COMPRESS_RL:
        func_0210f9a8(packed, dest);
        break;
    }
    func_02104494(gGameHeap, packed);
    return unpackedSize;
}
