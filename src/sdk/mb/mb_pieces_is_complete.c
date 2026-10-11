#include "types.h"
#include "sdk/mb.h"

extern DlPlayFragmentStore *data_022acf04;
extern DlPlayFragmentGeometry data_022acf08;

/* 1 when every piece of the request from child aid has arrived. */
int MbPieces_IsComplete(int aid)
{
    u16 i = 0;

    if (data_022acf08.pieceCount > 0) {
        u32 received = data_022acf04->received[aid - 1];

        do {
            if (!((1 << i) & received)) {
                return 0;
            }
            i++;
        } while (i < data_022acf08.pieceCount);
    }
    return 1;
}
