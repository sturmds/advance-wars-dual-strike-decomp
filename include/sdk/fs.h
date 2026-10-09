#ifndef SDK_FS_H
#define SDK_FS_H

#include "types.h"

/* Layout not recovered yet; only used through pointers. */
typedef struct FSArchive FSArchive;

typedef struct FSFileID {
    FSArchive *arc;
    u32 file_id;
} FSFileID;

#define FS_FILE_STATUS_IS_FILE 0x10

/* An open file, 0x44 bytes in this SDK version. */
typedef struct FSFile {
    u8 pad_00[0x0c];
    u32 flags;                         /* FS_FILE_STATUS_IS_FILE once opened as a file */
    u8 pad_10[0x20 - 0x10];
    u32 top;                           /* first byte of the file in the archive */
    u32 bottom;                        /* one past its last byte */
    u8 pad_28[0x44 - 0x28];
} FSFile;

/* One entry of the ROM's ARM9 overlay table (y9.bin), 32 bytes. */
typedef struct OverlayTableRow {
    u32 id;
    u8 *ram_address;
    u32 ram_size;
    u32 bss_size;
    void *sinit_init;
    void *sinit_init_end;
    u32 file_id;
    u32 compressed : 24;
    u32 flag : 8;
} OverlayTableRow;

typedef struct FSOverlayInfo {
    OverlayTableRow header;
} FSOverlayInfo;

/* The archive for the cartridge ROM file system. */
extern FSArchive gRomFileArchive;

FSFileID FS_GetOverlayFileID(const FSOverlayInfo *info);
void FS_InitFile(FSFile *file);
BOOL FS_OpenFile(FSFile *file, const char *path);
s32 FS_ReadFile(FSFile *file, void *dest, s32 size);
BOOL FS_CloseFile(FSFile *file);

#endif
