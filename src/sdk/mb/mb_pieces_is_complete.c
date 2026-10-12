#include "types.h"
#include "sdk/mb.h"

/* 1 when every piece of the request from child aid has arrived. */
int MbPieces_IsComplete(int aid) {
  u16 i = 0;

  if (gMbRecvPieceSize.pieceCount > 0) {
    u32 received = gMbRecvPieces->received[aid - 1];

    do {
      if (!((1 << i) & received)) {
        return 0;
      }
      i++;
    } while (i < gMbRecvPieceSize.pieceCount);
  }
  return 1;
}
