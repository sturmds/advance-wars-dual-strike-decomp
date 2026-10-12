#include "types.h"
#include "sdk/mb.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);
int MbPieces_IsComplete(int aid);
u8 *MbPieces_Add(DlPlayRequestFragment *piece, int aid);

/* Unpacks a message from child aid (src, bytes as received) into header.
 * Returns where the message's payload is, or NULL when there is none yet. A
 * piece of the child's request returns the whole request once the last piece
 * is in. */
u8 *MbComm_ReadChildMessage(const u8 *src, DlPlayChildMsgHead *header, int aid)
{
    u8 *payload;

    header->type = src[0];
    switch (header->type) {
    case DLPLAY_FROM_CHILD_ASK_FILE: {
        DlPlayRequestFragment *piece = (DlPlayRequestFragment *)header;

        if (MbPieces_IsComplete(aid)) {
            return gMbRecvPieces->data[aid - 1];
        }
        piece->index = src[1];
        if (piece->index > gMbRecvPieceSize.pieceCount) {
            return NULL;
        }
        Mem_CopyBytes(src + 2, piece->data, gMbRecvPieceSize.pieceSize);
        payload = MbPieces_Add(piece, aid);
        break;
    }
    case DLPLAY_FROM_CHILD_GOT_LAYOUT:
        header->request = src[1] & 0xff;
        header->request |= (src[2] << 8) & 0xff00;
        payload = (u8 *)src + 3;
        break;
    case DLPLAY_FROM_CHILD_NEXT_BLOCK:
        payload = (u8 *)src + 3;
        header->request = src[1] & 0xff;
        header->request |= (src[2] << 8) & 0xff00;
        Mem_CopyBytes(payload, header->pad_04, gMbRecvPieceSize.pieceSize);
        payload += gMbRecvPieceSize.pieceSize;
        break;
    default:
        return NULL;
    }
    return payload;
}
