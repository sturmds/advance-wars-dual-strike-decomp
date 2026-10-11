#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

extern DlPlayHostLinkArea *data_022aced4;

void Mem_CopyBytes(const void *src, void *dst, u32 size);
u8 *func_0211dfe4(u8 *recv, DlPlayChildMsgHead *header, u16 aid);
void MbComm_SetState(u16 aid, int state, void *arg);
u16 MbMaxU32(u16 nextBlock, u16 request);

/* Handles one child's message and moves the child to its next comm state:
 * file request, then accept or kick (as the application decided in
 * childRequests), download, and boot.
 * Needed to match: each file bitmap update reads the field through an
 * (int) index and a temporary, so the read and the write do not share one
 * address expression; the empty BOOTREQ_ACCEPTED case makes the jump table
 * start at type 0. */
void MbParent_HandleChildMessage(WMMpRecvData *recv, u16 aid)
{
    DlPlayChildMsgHead header;
    DlPlayChildFileAsk request;
    int state;
    u8 *data;
    u8 fileId;
    u16 bitmap;

    if (aid == 0 || aid > DLPLAY_CHILD_SLOTS) {
        return;
    }
    data = func_0211dfe4(recv->cdata, &header, aid);
    state = data_022aced4->commState[aid - 1];

    switch (header.type) {
    case DLPLAY_FROM_CHILD_ASK_FILE:
        if (state == DLPLAY_CHILD_JOINED) {
            if (data == NULL) {
                return;
            }
            Mem_CopyBytes(data, &request, DLPLAY_FILE_ASK_BYTES);
            data_022aced4->childGgids[aid - 1] = request.ggid;
            data_022aced4->childVersions[aid - 1] = request.version;
            Mem_CopyBytes(&request.user, &data_022aced4->childUsers[aid - 1], sizeof(DlPlayPlayerBadge));
            data_022aced4->childUsers[aid - 1].playerNo = (u8)aid;
            MbComm_SetState(aid, DLPLAY_CHILD_ASKED_FILE, &request.user);
        }
        if (state == DLPLAY_CHILD_ASKED_FILE) {
            u8 count = 0;
            u8 i;

            fileId = ((DlPlayChildFileAsk *)data)->fileId;
            if (fileId >= DLPLAY_OFFER_SLOTS || !data_022aced4->files[fileId].active
                || data_022aced4->childGgids[aid - 1] != data_022aced4->files[fileId].registry->ggid) {
                data_022aced4->childRequests[aid - 1] = DLPLAY_APP_WANTS_REFUSE;
            } else {
                for (i = 0; i < 16; i++) {
                    if (data_022aced4->files[fileId].childBitmap & (1 << i)) {
                        count++;
                    }
                }
                if (count >= data_022aced4->files[fileId].registry->maxPlayers) {
                    data_022aced4->childRequests[aid - 1] = DLPLAY_APP_WANTS_NOTHING;
                    MbComm_SetState(aid, DLPLAY_CHILD_NO_ROOM, NULL);
                    return;
                }
            }
            switch (data_022aced4->childRequests[aid - 1]) {
            case DLPLAY_APP_WANTS_ADMIT:
                if (data_022aced4->childEntryBitmap & (1 << aid)) {
                    return;
                }
                data_022aced4->childCount++;
                data_022aced4->childEntryBitmap |= 1 << aid;
                data_022aced4->childFiles[aid - 1] = fileId;
                bitmap = data_022aced4->files[(int)fileId].childBitmap;
                data_022aced4->files[fileId].childBitmap = bitmap | (1 << aid);
                bitmap = data_022aced4->files[(int)fileId].changedBitmap;
                data_022aced4->files[fileId].changedBitmap = bitmap | (1 << aid);
                data_022aced4->childRequests[aid - 1] = DLPLAY_APP_WANTS_NOTHING;
                MbComm_SetState(aid, DLPLAY_CHILD_ADMITTED, NULL);
                break;
            case DLPLAY_APP_WANTS_REFUSE:
                data_022aced4->childRequests[aid - 1] = DLPLAY_APP_WANTS_NOTHING;
                MbComm_SetState(aid, DLPLAY_CHILD_REFUSED, NULL);
                break;
            }
        }
        break;

    case DLPLAY_FROM_CHILD_GOT_LAYOUT:
        if (state == DLPLAY_CHILD_ADMITTED) {
            MbComm_SetState(aid, DLPLAY_CHILD_AWAITING_START, NULL);
        } else if (state == DLPLAY_CHILD_AWAITING_START
                   && data_022aced4->childRequests[aid - 1] == DLPLAY_APP_WANTS_UPLOAD) {
            fileId = data_022aced4->childFiles[aid - 1];
            bitmap = data_022aced4->files[(int)fileId].pollBitmap;
            data_022aced4->files[fileId].pollBitmap = bitmap | (1 << aid);
            data_022aced4->files[fileId].currentBlock = 0;
            data_022aced4->childRequests[aid - 1] = DLPLAY_APP_WANTS_NOTHING;
            MbComm_SetState(aid, DLPLAY_CHILD_RECEIVING, NULL);
        }
        break;

    case DLPLAY_FROM_CHILD_NEXT_BLOCK:
        if (state == DLPLAY_CHILD_RECEIVING) {
            fileId = data_022aced4->childFiles[aid - 1];
            if (fileId == 0xff) {
                return;
            }
            data_022aced4->files[fileId].nextBlock = MbMaxU32(data_022aced4->files[fileId].nextBlock, header.request);
        }
        break;

    case DLPLAY_FROM_CHILD_HALT:
        if (state == DLPLAY_CHILD_RECEIVING) {
            fileId = data_022aced4->childFiles[aid - 1];
            if (fileId == 0xff) {
                return;
            }
            bitmap = data_022aced4->files[(int)fileId].pollBitmap;
            data_022aced4->files[fileId].pollBitmap = bitmap & ~(1 << aid);
            MbComm_SetState(aid, DLPLAY_CHILD_HAS_PROGRAM, NULL);
        } else if (state == DLPLAY_CHILD_HAS_PROGRAM
                   && data_022aced4->childRequests[aid - 1] == DLPLAY_APP_WANTS_START) {
            data_022aced4->childRequests[aid - 1] = DLPLAY_APP_WANTS_NOTHING;
            MbComm_SetState(aid, DLPLAY_CHILD_TOLD_TO_BOOT, NULL);
        }
        break;

    case DLPLAY_FROM_CHILD_WILL_START:
        break;
    }
}
