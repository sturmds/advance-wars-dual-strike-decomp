#include "sdk/fs.h"

void FSi_ReadTable(ArchiveTableCursor *cursor, void *dest, u32 length);

/* One directory entry of the FNT, 8 bytes. */
typedef struct FSDirEntry {
    u32 entryStart;                    /* offset of the directory's listing in the FNT */
    u16 firstFileId;
    u16 parent;                        /* parent directory id; the low 12 bits are the id */
} FSDirEntry;

/* Archive command: moves an open directory to the requested position. A
 * request for the start of a directory (entry 0, offset 0) takes the
 * first file id and listing offset from the directory's FNT entry. */
int Archive_CmdSetDirPosition(FSFile *file)
{
    FSDirEntry entry;
    ArchiveTableCursor cursor;
    FSArchive *arc = file->arc;
    FSDirPos *target = &file->arg.seekDir.pos;

    cursor.arc = arc;
    cursor.pos = arc->fnt + target->ownId * 8;
    FSi_ReadTable(&cursor, &entry, 8);
    file->dirPos = *target;
    if (target->index == 0 && target->pos == 0) {
        file->dirPos.index = entry.firstFileId;
        file->dirPos.pos = arc->fnt + entry.entryStart;
    }
    file->parent = entry.parent & 0xfff;
    return FS_RESULT_SUCCESS;
}
