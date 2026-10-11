#ifndef SDK_FS_H
#define SDK_FS_H

#include "types.h"
#include "sdk/os.h"

typedef struct FSArchive FSArchive;
typedef struct FSFile FSFile;

/* Results of an archive command. */
#define FS_RESULT_SUCCESS 0
#define FS_RESULT_FAILURE 1
#define FS_RESULT_PROC_ASYNC 6             /* the archive finishes the command later */

/* Commands an archive runs for a file, in the order of its command table. */
#define ARCHIVE_OP_SET_DIR_POS 2
#define ARCHIVE_OP_OPEN_BY_ID 6
#define ARCHIVE_OP_OPEN_BY_EXTENT 7
#define ARCHIVE_OP_RELEASE_HANDLE 8

typedef struct FSFileID {
    FSArchive *arc;
    u32 file_id;
} FSFileID;

/* Low four bits of flags: set while a command runs on the file. */
#define FILE_HANDLE_BUSY_BITS 0xf
#define FS_FILE_STATUS_BUSY 0x4
#define FS_FILE_STATUS_IS_FILE 0x10

/* Position inside a directory listing. */
typedef struct FSDirPos {
    FSArchive *arc;
    u16 ownId;                         /* directory id in the FNT */
    u16 index;                         /* entry within the directory */
    u32 pos;                           /* byte offset of that entry in the FNT */
} FSDirPos;

/* An open file or directory, 0x44 bytes in this SDK version. */
struct FSFile {
    FSFile *prev;                      /* files of one archive form a list */
    FSFile *next;
    FSArchive *arc;
    u32 flags;                         /* FS_FILE_STATUS_IS_FILE once opened as a file */
    u32 command;                       /* ARCHIVE_OP_* being run */
    u32 error;                         /* ARCHIVE_STATUS_* of the last command */
    OSThreadQueue queue;               /* threads waiting for the command to end */
    union {
        struct {                       /* an open file */
            u32 id;                    /* file id within the archive */
            u32 top;                   /* first byte of the file in the archive */
            u32 bottom;                /* one past its last byte */
            u32 pos;                   /* read position in the archive */
        };
        struct {                       /* an open directory */
            FSDirPos dirPos;
            u32 parent;
        };
    };
    union {                            /* arguments of the running command */
        struct {
            u32 top;
            u32 bottom;
            u32 id;
        } openFileDirect;
        struct {
            u32 unused;
            u32 id;
        } openFileFast;
        struct {
            void *dest;
            u32 requested;
            u32 length;
        } readFile;
        struct {
            FSDirPos pos;
        } seekDir;
    } arg;
    u8 pad_38[0x44 - 0x38];
};

/* Where a table read starts. */
typedef struct ArchiveTableCursor {
    FSArchive *arc;
    u32 pos;                           /* offset in the archive */
} ArchiveTableCursor;

typedef int (*ArchiveByteReader)(FSArchive *arc, void *dest, u32 src, u32 len);
typedef int (*ArchiveByteWriter)(FSArchive *arc, const void *src, u32 dest, u32 len);

#define ARCHIVE_STATE_IN_LIST 0x1       /* in the list of archives */
#define ARCHIVE_STATE_ROUTINES_SET 0x2           /* tables and routines are set */
#define ARCHIVE_STATE_AWAITING_DRIVER 0x100          /* a command waits for Archive_OnCommandFinished */
#define ARCHIVE_STATE_CALLER_BLOCKED 0x200        /* a caller sleeps until the command ends */

/* A file archive: a file system with a file allocation table (FAT) and a
 * file name table (FNT) and routines to read it. 0x50 bytes. */
struct FSArchive {
    u32 name;                          /* up to four characters packed into one word */
    FSArchive *next;                   /* registered archives form a list */
    FSArchive *prev;
    OSThreadQueue queue;               /* threads waiting for the archive */
    u32 flags;
    u32 unk_14;
    FSFile *command;                   /* file whose command is running */
    u32 base;
    u32 fat;
    u32 fatSize;
    u32 fnt;
    u32 fntSize;
    u32 fatOriginal;                   /* fat as given when the archive was loaded */
    u32 fntOriginal;
    u32 unk_38;                        /* cleared when the archive is loaded */
    ArchiveByteReader read;
    ArchiveByteWriter write;
    ArchiveByteReader readTable;       /* copy of read, used for FAT and FNT reads */
    void *tableBuffer;                 /* where the tables are cached, or NULL */
    u32 tableBufferSize;
};

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
