#include "types.h"
#include "sdk/mb.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);
int MbPieces_IsComplete(int aid);

/* Stores one piece of child aid's request. Returns the child's buffer once
 * all pieces are in, otherwise NULL. */
u8 *MbPieces_Add(DlPlayRequestFragment *piece, int aid)
{
    u8 index;

    if (gMbRecvPieces == NULL) {
        return NULL;
    }
    index = piece->index;
    if (index > gMbRecvPieceSize.pieceCount) {
        return NULL;
    }
    Mem_CopyBytes(piece->data, gMbRecvPieces->data[aid - 1] + index * gMbRecvPieceSize.pieceSize,
                 gMbRecvPieceSize.pieceSize);
    gMbRecvPieces->received[aid - 1] |= 1 << index;
    if (MbPieces_IsComplete(aid)) {
        return gMbRecvPieces->data[aid - 1];
    }
    return NULL;
}
