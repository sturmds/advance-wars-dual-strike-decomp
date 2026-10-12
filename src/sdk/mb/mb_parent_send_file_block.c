#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayHostLinkArea *data_022aced4;

void MbParentFile_AdvanceBlock(int fileIndex);
BOOL MbFile_GetBlockInfo(DlPlayBlockSpot *info, const DlPlaySegmentStarts *table, u16 block, const DlPlayProgramLayout *header);
u8 *MbComm_WriteParentMessage(DlPlayHostMsgHead *header, u8 *buffer);
void MbCache_LoadPage(DlPlayWorkerJob *task);
int func_0211b458(int size, int bitmap, void *data);

/* Sends the next block of the next file that children are receiving. The
 * block comes from the file's cache; on a miss it starts a load of the
 * page into the oldest ready cache page and sends nothing.
 * The address computation reads the file through MbComm_GetFile to match. */
int MbParent_SendFileBlock(void)
{
    DlPlayHostMsgHead header;
    DlPlayBlockSpot block;
    u8 *data;
    u8 i;

    if (data_022aced4->fileCount == 0) {
        return DLPLAY_NOTHING_TRANSMITTED;
    }
    for (i = 0; i < DLPLAY_OFFER_SLOTS; i++) {
        data_022aced4->currentFile = (data_022aced4->currentFile + 1) % DLPLAY_OFFER_SLOTS;
        if (data_022aced4->files[data_022aced4->currentFile].active
            && data_022aced4->files[data_022aced4->currentFile].pollBitmap != 0) {
            break;
        }
    }
    if (i == DLPLAY_OFFER_SLOTS) {
        return DLPLAY_NOTHING_TRANSMITTED;
    }

    MbParentFile_AdvanceBlock(data_022aced4->currentFile);
    {
        DlPlayHostLinkArea *work = data_022aced4;

        if (!MbFile_GetBlockInfo(&block, &work->files[work->currentFile].blockTable,
                           work->files[work->currentFile].currentBlock, (DlPlayProgramLayout *)work->files[work->currentFile].dlFileInfo)) {
            return DLPLAY_NOTHING_TRANSMITTED;
        }
    }

    header.type = DLPLAY_TO_CHILD_BLOCK;
    header.fileId = data_022aced4->currentFile;
    header.block = data_022aced4->files[data_022aced4->currentFile].currentBlock;
    data = MbComm_WriteParentMessage(&header, data_022aced4->sendBuffer);
    {
        DlPlayHostLinkArea *work = data_022aced4;
        u32 address = block.offset - MbComm_GetFile(work, work->currentFile)->blockTable.segmentOffsets[block.segment]
                    + MbComm_GetFile(work, work->currentFile)->segmentRomAddresses[block.segment];
        DlPlayHeldProgram *cache = MbComm_GetFile(work, work->currentFile)->cache;

        if (!MbCache_Read(cache, address, data, block.size)) {
            DlPlayWorkerJob *task = &data_022aced4->task;

            if (!MbTask_IsBusy(task) && cache->readSinceLoad != 0) {
                DlPlayHeldBlock *pages = cache->pages;
                DlPlayHeldBlock *page = NULL;
                int j;

                for (j = 0; j < DLPLAY_HELD_BLOCK_SLOTS; j++) {
                    if (pages[j].state == DLPLAY_HELD_BLOCK_USABLE
                        && (page == NULL || page->address > pages[j].address)) {
                        page = &pages[j];
                    }
                }
                if (page == NULL) {
                    OS_Terminate();
                }
                cache->readSinceLoad = 0;
                page->state = DLPLAY_HELD_BLOCK_FILLING;
                page->address = address & ~31;
                task->page = page;
                task->cache = cache;
                MbTask_Set(task, MbCache_LoadPage, NULL, 4);
            }
            return DLPLAY_NOTHING_TRANSMITTED;
        }
    }
    return func_0211b458(block.size + DLPLAY_HOST_MSG_HEAD_BYTES,
                         data_022aced4->files[data_022aced4->currentFile].pollBitmap, data_022aced4->sendBuffer);
}
