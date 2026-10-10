#ifndef GAME_UNK_OV005_0236BB90_H
#define GAME_UNK_OV005_0236BB90_H

#include "types.h"
#include "game/task.h"

/* One of the 24-byte entries in the large overlay 5 work block. A first
 * halfword of 0 ends a list of entries. */
typedef struct Ov005SlideEntry {
    u16 id;
    u8 pad_02[0x10 - 0x02];
    s16 pos;                           /* moved by the slide tasks */
    u8 pad_12[0x14 - 0x12];
    Task *slideTask;                   /* task that moves pos */
} Ov005SlideEntry;

#define OV005_SLIDE_ENTRIES 17

/* Overlay 5 work block (0x97e0 bytes) that Ov005Screen_AllocLargeWork
 * allocates into Ov005Task.work and data_ov005_0236bb90. Unk_ov005_0235a30c
 * and the palette data at 0x8360 probably belong to the same block. */
typedef struct Unk_ov005_0236bb90 {
    u8 pad_0000[0x8018];
    Ov005SlideEntry entriesA[OV005_SLIDE_ENTRIES];
    Ov005SlideEntry entriesB[OV005_SLIDE_ENTRIES];
    u8 pad_8348[0x97e0 - 0x8348];
} Unk_ov005_0236bb90;

#endif
