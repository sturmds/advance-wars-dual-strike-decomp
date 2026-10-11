#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayHostLinkArea *data_022aced4;

void MbParentFile_AdvanceBlock(u8 fileId);
BOOL func_0211a7d8(DlPlayBlockSpot *block, u32 *segmentOffsets, u16 blockNo, u8 *dlFileInfo);
u8 *func_0211e120(DlPlayHostMsgHead *header, u8 *buffer);
BOOL MbSegment_Read(DlPlayHeldProgram *cache, u32 address, u8 *dst, u32 size);
BOOL func_0211dcec(DlPlayWorkerJob *task);
void func_0211db5c(DlPlayWorkerJob *task, void (*callback)(DlPlayWorkerJob *task), int arg, int priority);
void MbCache_LoadPage(DlPlayWorkerJob *task);
int func_0211b458(u32 size, u16 bitmap, u8 *buffer);

/* Sends the next block of the next file that children are receiving. The
 * block comes from the file's cache; on a miss it starts a load of the
 * page into the oldest ready cache page and sends nothing.
 * Not matched yet (96.55%): in the address computation the original keeps
 * the work pointer in r5 and the file index in r2; ours uses r7 and r5. */
int func_021193fc(void)
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

        if (!func_0211a7d8(&block, work->files[work->currentFile].segmentOffsets,
                           work->files[work->currentFile].currentBlock, work->files[work->currentFile].dlFileInfo)) {
            return DLPLAY_NOTHING_TRANSMITTED;
        }
    }

    header.type = DLPLAY_TO_CHILD_BLOCK;
    header.fileId = data_022aced4->currentFile;
    header.block = data_022aced4->files[data_022aced4->currentFile].currentBlock;
    data = func_0211e120(&header, data_022aced4->sendBuffer);
    {
        DlPlayHostLinkArea *work = data_022aced4;
        u32 address = block.offset - work->files[work->currentFile].segmentOffsets[block.segment]
                    + work->files[work->currentFile].segmentAddresses[block.segment];
        DlPlayHeldProgram *cache = work->files[work->currentFile].cache;

        if (!MbSegment_Read(cache, address, data, block.size)) {
            DlPlayWorkerJob *task = &data_022aced4->task;

            if (!func_0211dcec(task) && cache->loadPending != 0) {
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
                cache->loadPending = 0;
                page->state = 1;
                page->address = address & ~31;
                task->page = page;
                task->cache = cache;
                func_0211db5c(task, MbCache_LoadPage, 0, 4);
            }
            return DLPLAY_NOTHING_TRANSMITTED;
        }
    }
    return func_0211b458(block.size + DLPLAY_HOST_MSG_HEAD_BYTES,
                         data_022aced4->files[data_022aced4->currentFile].pollBitmap, data_022aced4->sendBuffer);
}
