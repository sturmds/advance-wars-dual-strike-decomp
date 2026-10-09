#ifndef GAME_LINK_BUFFERS_H
#define GAME_LINK_BUFFERS_H

#include "types.h"

/* Link play buffers that Link_ResetBuffers resets. Each has two bytes of
 * bookkeeping after its data, both cleared together; what they mean is not
 * worked out yet. */

typedef struct Unk_0229ca88 {
    u8 pad_00[0xc0];
    u8 unk_c0;
    u8 unk_c1;
} Unk_0229ca88;

/* data_0229cdfc, 7 entries */
typedef struct Unk_0229cdfc {
    u8 pad_000[0x180];
    u8 unk_180;
    u8 unk_181;
} Unk_0229cdfc;

/* data_0229cb4c, 7 entries */
typedef struct Unk_0229cb4c {
    u8 pad_00[0x60];
    u8 unk_60;
    u8 unk_61;
} Unk_0229cb4c;

#endif
