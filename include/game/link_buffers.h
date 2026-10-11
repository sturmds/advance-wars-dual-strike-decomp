#ifndef GAME_LINK_BUFFERS_H
#define GAME_LINK_BUFFERS_H

#include "types.h"

/* Link play buffers that Link_ResetBuffers resets. Each has two bytes of
 * bookkeeping after its data, both cleared together; what they mean is not
 * worked out yet. */

/* One queued packet: the first 24 bytes of a LinkPacket. */
typedef struct LinkQueueSlot {
    u8 bytes[24];
} LinkQueueSlot;

/* Each queue below is a ring buffer. The first of its two trailing bytes is
 * the read index, the second the write index; the queue is empty when they
 * are equal and full when the write index is one behind the read index. */

/* data_0229ca88: packets waiting to be sent, 8 slots */
typedef struct Unk_0229ca88 {
    LinkQueueSlot slots[8];
    u8 unk_c0;
    u8 unk_c1;
} Unk_0229ca88;

/* data_0229cdfc, 7 entries, one per peer: 16 slots */
typedef struct Unk_0229cdfc {
    LinkQueueSlot slots[16];
    u8 unk_180;
    u8 unk_181;
} Unk_0229cdfc;

/* data_0229cb4c, 7 entries, one per peer: 4 slots */
typedef struct Unk_0229cb4c {
    LinkQueueSlot slots[4];
    u8 unk_60;
    u8 unk_61;
} Unk_0229cb4c;

#endif
