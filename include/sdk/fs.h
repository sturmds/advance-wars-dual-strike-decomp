#ifndef SDK_FS_H
#define SDK_FS_H

#include "types.h"
#include "sdk/os.h"

typedef struct FSArchive FSArchive;
typedef struct FSFile FSFile;

/* Results of an archive command. */
#define FS_RESULT_SUCCESS 0
#define FS_RESULT_FAILURE 1
#define FS_RESULT_BUSY 2                   /* the command has not finished */
#define FS_RESULT_CANCELED 3
#define FS_RESULT_PROC_ASYNC 6             /* the archive finishes the command later */
#define FS_RESULT_PROC_DEFAULT 7           /* run the default command instead */

/* Commands an archive runs for a file, in the order of its command table. */
#define ARCHIVE_OP_COPY_OUT 0
#define FS_RESULT_UNSUPPORTED 4
#define FS_RESULT_PROC_UNKNOWN 8           /* the archive does not handle the command */

/* Commands an archive runs for a file, in the order of its command table. */
#define FS_COMMAND_INVALID 14
#define ARCHIVE_OP_COPY_IN 1
#define ARCHIVE_OP_SET_DIR_POS 2
#define ARCHIVE_OP_NEXT_DIR_ENTRY 3
#define ARCHIVE_OP_RESOLVE_PATH 4
#define ARCHIVE_OP_BUILD_FULL_PATH 5
#define ARCHIVE_OP_OPEN_BY_ID 6
#define ARCHIVE_OP_OPEN_BY_EXTENT 7
#define ARCHIVE_OP_RELEASE_HANDLE 8
#define FS_COMMAND_ACTIVATE 9
#define FS_COMMAND_IDLE 10

/* Commands that always run synchronously: seek directory to close file. */
#define ARCHIVE_OP_BLOCKING_SET 0x1fc

typedef struct FSFileID {
    FSArchive *arc;
    u32 file_id;
} FSFileID;

/* Low four bits of flags: set while a command runs on the file. */
#define FILE_HANDLE_BUSY_BITS 0xf
#define FILE_HANDLE_QUEUED 0x01        /* a command is queued or running (tested by FS_IsBusy) */
#define FS_FILE_STATUS_CANCEL 0x02
#define FS_FILE_STATUS_SYNC 0x04           /* a thread waits for the command */
#define FS_FILE_STATUS_ASYNC 0x08          /* the archive runs the command in the background */
#define FS_FILE_STATUS_IS_FILE 0x10
#define FS_FILE_STATUS_IS_DIR 0x20

/* Where FS_SeekFile counts the offset from. */
#define FS_SEEK_SET 0
#define FS_SEEK_CUR 1
#define FS_SEEK_END 2

/* Position inside a directory listing. */
typedef struct FSDirPos {
    FSArchive *arc;
    u16 ownId;                         /* directory id in the FNT */
    u16 index;                         /* entry within the directory */
    u32 pos;                           /* byte offset of that entry in the FNT */
} FSDirPos;

/* Ids above 0xffff mean "none" in the path commands. */
#define FILE_ID_NONE 0x10000
#define FS_FILE_NAME_MAX 127

/* One entry of a directory listing, as the read-directory command fills it. */
typedef struct DirListingEntry {
    union {
        FSFileID fileId;               /* when the entry is a file */
        FSDirPos dirId;                /* when the entry is a directory */
    };
    u32 isDirectory;
    u32 nameLength;
    char name[FS_FILE_NAME_MAX + 1];
} DirListingEntry;

/* Arguments of the get-path command. */
typedef struct FSGetPathInfo {
    u8 *buffer;                        /* NULL to ask only for the length */
    u32 bufferSize;
    u16 totalLength;                   /* path length with its NUL, 0 until known */
    u16 dirId;                         /* directory of the file, cached with totalLength */
} FSGetPathInfo;

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
        struct {
            DirListingEntry *entry;
            BOOL skipName;             /* fill in nameLength but not name */
        } readDir;
        struct {
            FSDirPos pos;              /* where the search starts */
            const char *path;
            BOOL findDirectory;
            union {
                FSFileID *file;
                FSDirPos *dir;
            } result;
        } findPath;
        FSGetPathInfo getPath;
    } arg;
};

/* An archive's own handler for some commands, tried before the default ones. */
typedef int (*ArchiveCommandHook)(FSFile *file, u32 command);

/* The ternary under the != 0 is how the original compiles these tests. */
static inline BOOL FS_IsBusy(const FSFile *file)
{
    return ((file->flags & FILE_HANDLE_QUEUED) ? TRUE : FALSE) != 0;
}

static inline BOOL FileHandle_HasWaiter(const FSFile *file)
{
    return ((file->flags & FS_FILE_STATUS_SYNC) ? TRUE : FALSE) != 0;
}

/* Where a table read starts. */
typedef struct ArchiveTableCursor {
    FSArchive *arc;
    u32 pos;                           /* offset in the archive */
} ArchiveTableCursor;

typedef int (*ArchiveByteReader)(FSArchive *arc, void *dest, u32 src, u32 len);
typedef int (*ArchiveByteWriter)(FSArchive *arc, const void *src, u32 dest, u32 len);

#define ARCHIVE_NAME_MAX_CHARS 3

#define ARCHIVE_STATE_IN_LIST 0x1       /* in the list of archives */
#define ARCHIVE_STATE_ROUTINES_SET 0x2           /* tables and routines are set */
#define ARCHIVE_STATE_TABLES_CACHED 0x4     /* FAT and FNT copied into memory */
#define ARCHIVE_STATE_PAUSED 0x8        /* commands queue up but do not start */
#define ARCHIVE_STATE_BUSY 0x10         /* a command of the archive is running */
#define ARCHIVE_STATE_CANCEL_ASKED 0x20       /* FS_CancelFile was called for the running command */
#define ARCHIVE_STATE_PAUSE_ASKED 0x40      /* suspend once the running command ends */
#define ARCHIVE_STATE_TEARING_DOWN 0x80       /* new commands are canceled */
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

#define OVERLAY_STORED_PACKED 0x1     /* bit 0 of OverlayTableRow.flag */
#define OVERLAY_NEEDS_HMAC_CHECK 0x2   /* has a digest to check under Download Play */

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

typedef enum MIProcessor {
    MI_PROCESSOR_ARM9,
    MI_PROCESSOR_ARM7
} MIProcessor;

typedef struct FSOverlayInfo {
    OverlayTableRow header;
    u32 target;                        /* processor the overlay is for */
    u32 romTop;                        /* first byte of the overlay file in the ROM */
    u32 romSize;                       /* length of the overlay file */
} FSOverlayInfo;

/* SHA-1 HMAC of one overlay, checked before a downloaded program runs it. */
typedef struct OverlayHmacBytes {
    u8 bytes[20];
} OverlayHmacBytes;

/* The archive for the cartridge ROM file system. */
extern FSArchive gRomFileArchive;

FSFileID FS_GetOverlayFileID(const FSOverlayInfo *info);
FSArchive *FS_FindArchive(const char *name, int length);
u32 Archive_FoldNameIntoWord(const char *name, int length);
void FS_Init(u32 dmaNo);
void FS_InitFile(FSFile *file);
BOOL FS_LoadOverlayImage(const FSOverlayInfo *info);
BOOL FS_LoadOverlay(int target, u32 overlayId);
BOOL FS_UnloadOverlay(int target, u32 overlayId);
BOOL FS_OpenFile(FSFile *file, const char *path);
s32 FS_ReadFile(FSFile *file, void *dest, s32 size);
BOOL FS_CloseFile(FSFile *file);
BOOL FS_ChangeDir(const char *path);
BOOL FS_SeekFile(FSFile *file, s32 offset, int origin);
void FS_CancelFile(FSFile *file);
BOOL FS_WaitAsync(FSFile *file);
BOOL FS_OpenFileDirect(FSFile *file, FSArchive *arc, u32 top, u32 bottom, u32 index);

/* Not named yet: sends a command to the file's archive. */
BOOL FSi_SendCommand(FSFile *file, u32 command);

/* Not named yet: looks up a path and gives a file ID or a directory position. */
BOOL FSi_FindPath(FSFile *scratch, const char *path, FSFileID *fileId, FSDirPos *dirPos);
void FSi_ReleaseCommand(FSFile *file, int result);
void FSi_AppendToList(FSFile *file, FSFile *head);

#endif
