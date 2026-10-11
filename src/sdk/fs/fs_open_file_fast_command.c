#include "sdk/fs.h"

void FSi_ReadTable(ArchiveTableCursor *cursor, void *dest, u32 length);
int FSi_TranslateCommand(FSFile *file, u32 command);

/* Archive command: looks the file id up in the FAT (8 bytes per file: top
 * and bottom), then runs the open-by-range command on it. Fails when the id
 * is past the end of the FAT. */
int FSi_OpenFileFastCommand(FSFile *file)
{
    u32 range[2];
    ArchiveTableCursor cursor;
    FSArchive *arc = file->arc;
    u32 id = file->arg.openFileFast.id;
    u32 entry = id * 8;

    if (entry >= arc->fatSize) {
        return FS_RESULT_FAILURE;
    }
    cursor.arc = arc;
    cursor.pos = arc->fat + entry;
    FSi_ReadTable(&cursor, range, 8);
    file->arg.openFileDirect.top = range[0];
    file->arg.openFileDirect.bottom = range[1];
    file->arg.openFileDirect.id = id;
    return FSi_TranslateCommand(file, ARCHIVE_OP_OPEN_BY_EXTENT);
}
