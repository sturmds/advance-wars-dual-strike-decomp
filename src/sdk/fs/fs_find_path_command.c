#include "sdk/fs.h"

int FsDir_SyncJumpToDirStart(FSFile *file, int dirId);
int FSi_StrNICmp(const char *a, const char *b, int length);
int FSi_TranslateCommand(FSFile *file, u32 command);

static inline BOOL Path_IsPartOfName(u32 c)
{
    BOOL result = FALSE;

    if (c != '\0' && c != '/' && c != '\\') {
        result = TRUE;
    }
    return result;
}

/* Archive command: follows a path ("dir/sub/file", '/' or '\\' between
 * names, "." and ".." allowed) from arg.findPath.pos and writes the id of
 * the file or directory it names to arg.findPath.result. Names compare
 * without case. */
int FSi_FindPathCommand(FSFile *file)
{
    DirListingEntry entry;
    const char *path = file->arg.findPath.path;
    BOOL findDirectory = file->arg.findPath.findDirectory;

    FSi_TranslateCommand(file, ARCHIVE_OP_SET_DIR_POS);
    while (*path != '\0') {
        u32 c;
        int length;
        BOOL isDirectory;

        for (length = 0; Path_IsPartOfName(c = path[length]); length++) {
        }
        /* a name followed by a slash names a directory; needed to match:
         * the flag is built in c */
        if (c != '\0' || findDirectory) {
            c = TRUE;
        }
        isDirectory = c;
        if (length == 0) {
            return FS_RESULT_FAILURE;
        }
        if (*path == '.') {
            if (length == 1) {
                path += 1;
                goto next;
            }
            /* needed to match: '&' reads path[1] before the length test */
            if ((length == 2) & (path[1] == '.')) {
                if (file->dirPos.ownId != 0) {
                    FsDir_SyncJumpToDirStart(file, file->parent);
                }
                path += 2;
                goto next;
            }
        }
        if (length > FS_FILE_NAME_MAX) {
            return FS_RESULT_FAILURE;
        }
        file->arg.readDir.entry = &entry;
        file->arg.readDir.skipName = FALSE;
        for (;;) {
            if (FSi_TranslateCommand(file, ARCHIVE_OP_NEXT_DIR_ENTRY) != FS_RESULT_SUCCESS) {
                return FS_RESULT_FAILURE;
            }
            if (isDirectory == entry.isDirectory && length == entry.nameLength
                && FSi_StrNICmp(path, entry.name, length) == 0) {
                break;
            }
        }
        if (isDirectory) {
            file->arg.seekDir.pos = entry.dirId;
            path += length;
            FSi_TranslateCommand(file, ARCHIVE_OP_SET_DIR_POS);
        } else {
            if (findDirectory) {
                return FS_RESULT_FAILURE;
            }
            *file->arg.findPath.result.file = entry.fileId;
            return FS_RESULT_SUCCESS;
        }
    next:
        path += (*path != '\0') ? 1 : 0;
    }
    if (!findDirectory) {
        return FS_RESULT_FAILURE;
    }
    *file->arg.findPath.result.dir = file->dirPos;
    return FS_RESULT_SUCCESS;
}
