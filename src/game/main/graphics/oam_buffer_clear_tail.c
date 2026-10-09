#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/oam_buffer.h"

extern Unk_027c0280 *data_027c0280;

void func_02002d3c(void *entries, int count);   /* moves OAM entries off screen (y = 192), 16 at a time */

/* Throws away the sprites queued in the tail of the OAM buffer without
 * uploading them. The first store to unk_608 is dead but the build keeps it. */
void OamBuffer_ClearTail(void)
{
    func_02002d3c(data_027c0280->oamBuffer->tail.src, data_027c0280->oamBuffer->tail.count);
    data_027c0280->oamBuffer->unk_600 = data_027c0280->oamBuffer->tail.src;
    data_027c0280->oamBuffer->unk_608 = data_027c0280->oamBuffer;
    data_027c0280->oamBuffer->unk_608 = 0;
}
