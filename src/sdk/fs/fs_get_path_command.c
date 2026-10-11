#include "sdk/fs.h"

void Mem_CopyBytes(const void *src, void *dest, u32 size);
int FsDir_SyncJumpToDirStart(FSFile *file, int dirId);
int FSi_TranslateCommand(FSFile *file, u32 command);

extern const char gArchiveNameColonSlash[]; /* ":/" */

static inline BOOL FileHandle_IsDirectory(FSFile *file)
{
    return (file->flags & FS_FILE_STATUS_IS_DIR) ? TRUE : FALSE;
}

/* Archive command: writes the full path ("rom:/dir/file") of an open file
 * or directory into arg.getPath.buffer. A first call finds the length and
 * the file's directory and caches both. The path is built from its end:
 * each step looks up the current directory's name in its parent. */
int FSi_GetPathCommand(FSFile *file)
{
    DirListingEntry entry;
    FSFile walk;
    FSArchive *arc = file->arc;
    FSGetPathInfo *info = &file->arg.getPath;
    u32 dirId;
    u32 fileId;
    u32 id;
    u32 length;
    u8 *buffer;

    FS_InitFile(&walk);
    walk.arc = file->arc;
    if (FileHandle_IsDirectory(file) != FALSE) {
        dirId = file->dirPos.ownId;
        fileId = FILE_ID_NONE;
    } else {
        fileId = file->id;
        if (info->totalLength != 0) {
            dirId = info->dirId;
        } else {
            u32 dir;
            u32 dirCount;

            /* search every directory for the file; needed to match:
             * dirCount is set from dir */
            dir = 0;
            dirCount = dir;
            dirId = FILE_ID_NONE;
            do {
                FsDir_SyncJumpToDirStart(&walk, dir);
                if (dir == 0) {
                    /* the root's parent field holds the number of directories */
                    dirCount = walk.parent;
                }
                walk.arg.readDir.entry = &entry;
                walk.arg.readDir.skipName = TRUE;
                while (FSi_TranslateCommand(&walk, ARCHIVE_OP_NEXT_DIR_ENTRY) == FS_RESULT_SUCCESS) {
                    if (!entry.isDirectory && entry.fileId.file_id == fileId) {
                        dirId = walk.dirPos.ownId;
                        break;
                    }
                }
                if (dirId != FILE_ID_NONE) {
                    break;
                }
            } while (++dir < dirCount);
        }
    }
    if (dirId == FILE_ID_NONE) {
        info->totalLength = 0;
        return FS_RESULT_FAILURE;
    }

    if (info->totalLength == 0) {
        /* archive name, ":/", file name, then each directory name and its '/' */
        length = 0;
        if (arc->name <= 0xff) {
            length += 1;
        } else if (arc->name <= 0xff00) {
            length += 2;
        } else {
            length += 3;
        }
        length += 2;
        if (fileId != FILE_ID_NONE) {
            length += entry.nameLength;
        }
        id = dirId;
        if (id != 0) {
            FsDir_SyncJumpToDirStart(&walk, id);
            do {
                FsDir_SyncJumpToDirStart(&walk, walk.parent);
                walk.arg.readDir.entry = &entry;
                walk.arg.readDir.skipName = TRUE;
                while (FSi_TranslateCommand(&walk, ARCHIVE_OP_NEXT_DIR_ENTRY) == FS_RESULT_SUCCESS) {
                    if (entry.isDirectory && entry.dirId.ownId == id) {
                        length += entry.nameLength + 1;
                        break;
                    }
                }
                id = walk.dirPos.ownId;
            } while (id != 0);
        }
        info->totalLength = length + 1;
        info->dirId = dirId;
    }

    buffer = info->buffer;
    if (buffer == NULL) {
        return FS_RESULT_SUCCESS;
    }
    length = info->totalLength;
    if (info->bufferSize < length) {
        return FS_RESULT_FAILURE;
    }
    {
        /* needed to match: the compiler keeps pos as a register */
        u32 pos = 0;
        u32 nameLength;

        if (arc->name <= 0xff) {
            nameLength = 1;
        } else if (arc->name <= 0xff00) {
            nameLength = 2;
        } else {
            nameLength = 3;
        }

        Mem_CopyBytes(arc, buffer + pos, nameLength);
        pos += nameLength;
        Mem_CopyBytes(gArchiveNameColonSlash, buffer + pos, 2);
    }
    FsDir_SyncJumpToDirStart(&walk, dirId);
    if (fileId != FILE_ID_NONE) {
        u32 nameLength;

        walk.arg.readDir.entry = &entry;
        walk.arg.readDir.skipName = FALSE;
        while (FSi_TranslateCommand(&walk, ARCHIVE_OP_NEXT_DIR_ENTRY) == FS_RESULT_SUCCESS) {
            if (!entry.isDirectory && entry.fileId.file_id == fileId) {
                break;
            }
        }
        nameLength = entry.nameLength + 1;
        Mem_CopyBytes(entry.name, buffer + length - nameLength, nameLength);
        length -= nameLength;
    } else {
        *(buffer + length - 1) = '\0';
        length -= 1;
    }
    id = dirId;
    while (id != 0) {
        FsDir_SyncJumpToDirStart(&walk, walk.parent);
        walk.arg.readDir.entry = &entry;
        walk.arg.readDir.skipName = FALSE;
        *(buffer + length - 1) = '/';
        length -= 1;
        while (FSi_TranslateCommand(&walk, ARCHIVE_OP_NEXT_DIR_ENTRY) == FS_RESULT_SUCCESS) {
            if (entry.isDirectory && entry.dirId.ownId == id) {
                u32 nameLength = entry.nameLength;

                Mem_CopyBytes(entry.name, buffer + length - nameLength, nameLength);
                length -= nameLength;
                break;
            }
        }
        id = walk.dirPos.ownId;
    }
    return FS_RESULT_SUCCESS;
}
