#ifndef SDK_FS_H
#define SDK_FS_H

#include "types.h"

/* Layout not recovered yet; only used through pointers. */
typedef struct FSArchive FSArchive;

typedef struct FSFileID {
    FSArchive *arc;
    u32 file_id;
} FSFileID;

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

#endif
