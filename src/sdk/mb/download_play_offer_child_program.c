#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayHostLinkArea *data_022aced4;
extern u8 gMbGameSequence;

BOOL DownloadPlay_ParentRunning(void);
void MbFile_MakeDownloadInfo(void *downloadInfo, const void *buffer);
BOOL MbFile_MakeBlockTable(DlPlaySegmentStarts *table, const DlPlayProgramLayout *header);
void MbGame_Init(DlPlayGameEntry *game, const DlPlayGameSource *registry, const void *userParam);
void MbBeacon_AppendGame(DlPlayGameEntry *game);
void MbTask_Start(void *stack, u32 size);   /* start the task thread */

/* Adds a child program to the files the parent offers and puts its game on
 * the beacon. buffer holds the program's ROM header, auth code, cache
 * and card map, read by DownloadPlay_LoadChildProgram. Fails when MB has not started, the
 * registry is already in use or all 16 slots are full. */
BOOL DownloadPlay_OfferChildProgram(const DlPlayGameSource *registry, const void *buffer)
{
    DlPlayProgramLayout *downloadInfo;
    u8 fileId = 0xff;
    u8 i;
    int state = OS_DisableInterrupts();

    if (!DownloadPlay_ParentRunning()) {
        OS_RestoreInterrupts(state);
        return FALSE;
    }
    if (data_022aced4->fileCount + 1 > DLPLAY_OFFER_SLOTS) {
        OS_RestoreInterrupts(state);
        return FALSE;
    }
    for (i = 0; i < DLPLAY_OFFER_SLOTS; i++) {
        if (data_022aced4->files[i].registry == registry) {
            OS_RestoreInterrupts(state);
            return FALSE;
        }
        if (data_022aced4->files[i].active == 0) {
            fileId = i;
            break;
        }
    }
    if (i == DLPLAY_OFFER_SLOTS) {
        OS_RestoreInterrupts(state);
        return FALSE;
    }

    data_022aced4->files[fileId].registry = registry;
    downloadInfo = (DlPlayProgramLayout *)data_022aced4->files[fileId].dlFileInfo;
    MbFile_MakeDownloadInfo(downloadInfo, buffer);
    if (!MbFile_MakeBlockTable(&data_022aced4->files[fileId].blockTable, downloadInfo)) {
        OS_RestoreInterrupts(state);
        return FALSE;
    }
    MbGame_Init(&data_022aced4->files[fileId].gameInfo, registry, &data_022aced4->user);
    data_022aced4->files[fileId].gameInfo.unk_4b5 = fileId;
    MbBeacon_AppendGame(&data_022aced4->files[fileId].gameInfo);
    data_022aced4->files[fileId].gameInfo.unk_4b3 = gMbGameSequence++;
    data_022aced4->files[fileId].childBitmap = 1;
    data_022aced4->files[fileId].source = buffer;
    data_022aced4->files[fileId].cache = (DlPlayHeldProgram *)((u8 *)buffer + DLPLAY_SOURCE_HELD_AT);
    data_022aced4->files[fileId].segmentRomAddresses = (u32 *)((u8 *)buffer + DLPLAY_SOURCE_ROM_OFFSETS_AT);
    if (data_022aced4->files[fileId].cache->pages[3].state != 0 && !MbTask_IsAvailable()) {
        MbTask_InitInfo(&data_022aced4->task);
        MbTask_Start(data_022aced4->taskThreadStack, DLPLAY_WORKER_STACK_BYTES);
    }
    data_022aced4->files[fileId].active = 1;
    data_022aced4->fileCount++;
    OS_RestoreInterrupts(state);
    return TRUE;
}
