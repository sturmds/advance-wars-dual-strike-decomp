#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"
#include "sdk/wm.h"

extern DlPlayHostLinkArea *data_022aced4;
extern DlPlayStationArea *data_022aced8;
extern void *data_022acedc;

void MIi_CpuClear16(u16 value, void *dst, u32 size);
void MI_CpuFill8(void *dst, u8 value, u32 size);
BOOL PXI_IsCallbackReady(int tag, int proc);
void MbPieces_SetSize(u32 childMaxSize);
void func_0211e1f4(void *buffer);
void MbParent_CommCallback(u32 type, void *arg);
void MbParent_WmCallback(void);
void MbBeacon_Init(void);
int MbParent_InitWm(void);

/* Starts the Download Play parent on a channel: clears the per-child and
 * per-file state, sizes the WM MP buffers from the packet sizes, then brings
 * WM up (MbParent_InitWm). Returns MbParent_InitWm's result. */
int MbParent_StartCore(int channel)
{
    int i;
    int result;
    int enabled;
    volatile u16 zero;
    volatile u16 zero2;

    enabled = OS_DisableInterrupts();
    data_022aced8->channel = channel;
    /* The WM work area follows the comm work, 32-byte aligned. */
    data_022acedc = (void *)(((u32)data_022aced4 + sizeof(DlPlayHostLinkArea) + 31) & ~31);
    {
        DlPlayLinkEventFunc callback = data_022aced4->callback;

        zero = 0;
        MIi_CpuClear16(zero, data_022aced4->childUsers,
                       sizeof(DlPlayHostLinkArea) - 0x1340);   /* from childUsers to the end */
        MbComm_SetCallback(callback);
    }
    data_022aced4->blockSizeMax = data_022aced8->sendSize - DLPLAY_HOST_MSG_HEAD_BYTES;
    MbPieces_SetSize(data_022aced8->unk_502);
    func_0211e1f4(data_022aced4->pieceBuffer);
    for (i = 0; i < DLPLAY_CHILD_SLOTS; i++) {
        data_022aced4->commState[i] = 0;
        ((s8 *)data_022aced4->childFiles)[i] = -1;   /* no file picked; the SDK field is signed */
    }
    data_022aced4->fileCount = 0;
    zero2 = 0;
    MIi_CpuClear16(zero2, data_022aced4->files, sizeof(data_022aced4->files));
    MI_CpuFill8(data_022aced4->childRequests, 0, sizeof(data_022aced4->childRequests));

    data_022aced8->unk_524 = 1;
    data_022aced8->callback = MbParent_CommCallback;
    data_022aced8->wmCallback = MbParent_WmCallback;
    /* WM's MP buffer sizes: a 4-byte header per packet, 32-byte aligned; the
     * receive buffer holds one packet from each child, twice. */
    data_022aced8->parentMaxSize = data_022aced8->sendSize;
    data_022aced8->mpSendSize = (data_022aced8->parentMaxSize + 35) & ~31;
    data_022aced8->childMaxSize = data_022aced8->unk_502;
    data_022aced8->mpRecvSize =
        (((data_022aced8->childMaxSize + 14) * DLPLAY_CHILD_SLOTS + 41) & ~31) * 2;

    MbBeacon_Init();
    result = MbParent_InitWm();
    OS_RestoreInterrupts(enabled);
    data_022aced4->unk_74c8 = PXI_IsCallbackReady(PXI_FIFO_TAG_WVR, PXI_PROC_ARM7);
    return result;
}
