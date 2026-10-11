#include "types.h"
#include "sdk/mb.h"

extern DlPlayFragmentGeometry data_022acf08;

/* Sets how a child's request is cut up for messages of messageSize bytes: a
 * piece keeps two bytes of the message for its header. */
void MbPieces_SetSize(int messageSize)
{
    data_022acf08.pieceSize = messageSize - 2;
    data_022acf08.pieceCount = DLPLAY_FRAGMENT_TOTAL_BYTES / data_022acf08.pieceSize;
    data_022acf08.dataSize = DLPLAY_FRAGMENT_TOTAL_BYTES;
}
