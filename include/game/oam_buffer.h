#ifndef GAME_OAM_BUFFER_H
#define GAME_OAM_BUFFER_H

#include "types.h"

#define OAM_ENTRY_COUNT 128

/* Part of the OAM buffer. OamBuffer_Split splits the 128 entries in two:
 * the first headCount entries (head) and the rest (tail). */
typedef struct OamRegion {
    void *src;                         /* first entry of the region in the buffer */
    void *dst;                         /* same entry in the OAM copy that is uploaded */
    u16 byteOffset;                    /* where the region starts, 8 bytes per entry */
    u16 count;                         /* entries in the region */
    u32 unk_0c;
} OamRegion;

/* Pointed to by Unk_027c0280.oamBuffer. Starts with the sprite entries. */
typedef struct OamBuffer {
    u8 entries[0x600];                 /* 128 entries of 8 bytes, then affine data */
    void *unk_600;                     /* reset to tail.src when the tail is flushed or cleared */
    u8 pad_604[0x608 - 0x604];
    void *unk_608;                     /* cleared with it */
    u8 pad_60c[0x610 - 0x60c];
    OamRegion tail;
    OamRegion head;
} OamBuffer;

#endif
