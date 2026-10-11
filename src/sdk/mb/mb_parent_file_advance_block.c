#include "types.h"
#include "sdk/mb.h"

extern DlPlayHostLinkArea *data_022aced4;

/* Needed to match: the address is rebuilt at each use as
 * (work + index * file size)->files[0], not as work->files[index]. */
static inline DlPlayOfferedFile *GetFile(int fileIndex)
{
    return ((DlPlayHostLinkArea *)((u8 *)data_022aced4 + fileIndex * sizeof(DlPlayOfferedFile)))->files;
}

/* Moves a file's current block on for the next send. It steps forward by one
 * while at most 2 past nextBlock, and otherwise goes to nextBlock. */
void MbParentFile_AdvanceBlock(int fileIndex)
{
    if (!GetFile(fileIndex)->active) {
        return;
    }
    if (GetFile(fileIndex)->pollBitmap == 0) {
        return;
    }
    if (GetFile(fileIndex)->nextBlock <= GetFile(fileIndex)->currentBlock) {
        if (GetFile(fileIndex)->currentBlock <= GetFile(fileIndex)->nextBlock + 2) {
            GetFile(fileIndex)->currentBlock++;
            return;
        }
    }
    GetFile(fileIndex)->currentBlock = GetFile(fileIndex)->nextBlock;
}
