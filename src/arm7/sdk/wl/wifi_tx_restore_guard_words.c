#include "sdk/wl.h"

/*
 * Checks the two marks just before the queue's frame. When they were
 * overwritten, writes them again and turns the frame into a 1-byte frame
 * whose frameControl and duration hold the marks, counts it and returns TRUE.
 */
BOOL WifiTx_RestoreGuardWords(WlTxQueue *queue)
{
    WlRamFrame *frame = queue->frame;
    u16 *marks = (u16 *)frame - 2;
    u16 *header;

    if (marks[0] != WIFI_GUARD_WORD_A || marks[1] != WIFI_GUARD_WORD_B) {
        frame->length = 1;
        marks[0] = WIFI_GUARD_WORD_A;
        marks[1] = WIFI_GUARD_WORD_B;
        /* needed to match: frameControl and duration written through one pointer */
        header = &queue->frame->frameControl;
        header[0] = WIFI_GUARD_WORD_A;
        header[1] = WIFI_GUARD_WORD_B;
        WIFI_DRIVER_STATE->state.unk_ce++;
        return TRUE;
    }
    return FALSE;
}
