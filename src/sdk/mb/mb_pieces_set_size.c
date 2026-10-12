#include "types.h"
#include "sdk/mb.h"

/* Sets how a child's request is cut up for messages of messageSize bytes: a
 * piece keeps two bytes of the message for its header. */
void MbPieces_SetSize(int messageSize) {
  gMbRecvPieceSize.pieceSize = messageSize - 2;
  gMbRecvPieceSize.pieceCount =
      DLPLAY_FRAGMENT_TOTAL_BYTES / gMbRecvPieceSize.pieceSize;
  gMbRecvPieceSize.dataSize = DLPLAY_FRAGMENT_TOTAL_BYTES;
}
