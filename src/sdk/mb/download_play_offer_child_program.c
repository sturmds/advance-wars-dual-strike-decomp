#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayHostLinkAreaOld *data_022aced4;
extern u8 gMbGameSequence;

BOOL func_0211bdbc(void);              /* MB started */
void MbFile_MakeDownloadInfo(void *downloadInfo, const void *buffer);
BOOL MbFile_MakeBlockTable(void *blockTable, void *downloadInfo);
void MbGame_Init(DlPlayGameEntry *game, const DlPlayGameSource *registry, const void *userParam);
void MbBeacon_AppendGame(DlPlayGameEntry *game);
BOOL func_0211dd18(void);              /* task thread running */
void func_0211dd04(void *task);        /* clear a task */
void func_0211dd34(void *stack, u32 size);   /* start the task thread */

/* Adds a child program to the files the parent offers and puts its game on
 * the beacon. buffer holds the program's ROM header, auth code, cache list
 * and card map, read by DownloadPlay_LoadChildProgram. Fails when MB has not started, the
 * registry is already in use or all 16 slots are full. */
BOOL DownloadPlay_OfferChildProgram(const DlPlayGameSource *registry, const void *buffer)
{
    u8 *downloadInfo;
    u8 fileId = 0xff;
    u8 i;
    int state = OS_DisableInterrupts();

    if (!func_0211bdbc()) {
        OS_RestoreInterrupts(state);
        return FALSE;
    }
    if (data_022aced4->fileCount + 1 > DLPLAY_OFFER_SLOTS_OLD) {
        OS_RestoreInterrupts(state);
        return FALSE;
    }
    for (i = 0; i < DLPLAY_OFFER_SLOTS_OLD; i++) {
        if (data_022aced4->files[i].registry == registry) {
            OS_RestoreInterrupts(state);
            return FALSE;
        }
        if (data_022aced4->files[i].active == 0) {
            fileId = i;
            break;
        }
    }
    if (i == DLPLAY_OFFER_SLOTS_OLD) {
        OS_RestoreInterrupts(state);
        return FALSE;
    }

    data_022aced4->files[fileId].registry = registry;
    downloadInfo = data_022aced4->files[fileId].downloadInfo;
    MbFile_MakeDownloadInfo(downloadInfo, buffer);
    if (!MbFile_MakeBlockTable(data_022aced4->files[fileId].blockTable, downloadInfo)) {
        OS_RestoreInterrupts(state);
        return FALSE;
    }
    MbGame_Init(&data_022aced4->files[fileId].gameInfo, registry, data_022aced4->user);
    data_022aced4->files[fileId].gameInfo.unk_4b5 = fileId;
    MbBeacon_AppendGame(&data_022aced4->files[fileId].gameInfo);
    data_022aced4->files[fileId].gameInfo.unk_4b3 = gMbGameSequence++;
    data_022aced4->files[fileId].childFlags = 1;
    data_022aced4->files[fileId].source = buffer;
    data_022aced4->files[fileId].cacheList = (DlPlayKeptChunkSet *)((u8 *)buffer + DLPLAY_SOURCE_KEPT_LIST_AT);
    data_022aced4->files[fileId].cardMapping = (DlPlayProgramRomOffsets *)((u8 *)buffer + DLPLAY_SOURCE_ROM_OFFSETS_AT);
    if (data_022aced4->files[fileId].cacheList->list[3].state != 0 && !func_0211dd18()) {
        func_0211dd04(data_022aced4->currentTask);
        func_0211dd34(data_022aced4->taskThreadStack, DLPLAY_WORKER_STACK_BYTES);
    }
    data_022aced4->files[fileId].active = 1;
    data_022aced4->fileCount++;
    OS_RestoreInterrupts(state);
    return TRUE;
}
