#ifndef GAME_OV003_OBJECT_H
#define GAME_OV003_OBJECT_H

#include "types.h"

#define OV003_OBJECT_COUNT 80

/* One of the 80 14-byte entries in data_ov003_02369738 (overlay 3). A free
 * entry has active 0. */
typedef struct Ov003Object {
    s16 x;                             /* 8 per tile */
    s16 y;
    s16 offsetX;                       /* kind 7: where it ends up, relative to its start; 8 per tile */
    s16 offsetY;
    u16 travelled;                     /* distance moved so far (kind 7) */
    u8 unk_0a;
    u8 kind;                           /* 5 and 7 are drawn and updated apart from the others */
    u8 active;                         /* 0 = free entry; kind 7 also moves this far per frame */
    u8 variant;                        /* added to 10 for the sprite */
} Ov003Object;

typedef char Ov003Object_size_check[sizeof(Ov003Object) == 14 ? 1 : -1];

extern Ov003Object data_ov003_02369738[OV003_OBJECT_COUNT];

/* 20 bytes per object kind, in data_ov003_02364d8c. */
typedef struct Ov003ObjectKind {
    u8 range;                          /* how far the object moves, in 128-unit steps */
    u8 pad_01[19];
} Ov003ObjectKind;

extern Ov003ObjectKind data_ov003_02364d8c[];

#endif
