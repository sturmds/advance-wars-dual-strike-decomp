#include "types.h"
#include "sdk/mb.h"

extern DlPlayFragmentStore *data_022acf04;
extern DlPlayFragmentGeometry data_022acf08;

void Mem_CopyBytes(const void *src, void *dst, u32 size);
int MbPieces_IsComplete(int aid);

/* Stores one piece of child aid's request. Returns the child's buffer once
 * all pieces are in, otherwise NULL. */
u8 *MbPieces_Add(DlPlayRequestFragment *piece, int aid)
{
    u8 index;

    if (data_022acf04 == NULL) {
        return NULL;
    }
    index = piece->index;
    if (index > data_022acf08.pieceCount) {
        return NULL;
    }
    Mem_CopyBytes(piece->data, data_022acf04->data[aid - 1] + index * data_022acf08.pieceSize,
                 data_022acf08.pieceSize);
    data_022acf04->received[aid - 1] |= 1 << index;
    if (MbPieces_IsComplete(aid)) {
        return data_022acf04->data[aid - 1];
    }
    return NULL;
}
