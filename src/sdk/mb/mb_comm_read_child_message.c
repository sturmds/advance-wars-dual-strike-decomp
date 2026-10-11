#include "types.h"
#include "sdk/mb.h"

extern DlPlayFragmentStore *data_022acf04;
extern DlPlayFragmentGeometry data_022acf08;

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
            return data_022acf04->data[aid - 1];
        }
        piece->index = src[1];
        if (piece->index > data_022acf08.pieceCount) {
            return NULL;
        }
        Mem_CopyBytes(src + 2, piece->data, data_022acf08.pieceSize);
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
        Mem_CopyBytes(payload, header->pad_04, data_022acf08.pieceSize);
        payload += data_022acf08.pieceSize;
        break;
    default:
        return NULL;
    }
    return payload;
}
