#ifndef GAME_UNK_OV005_0236BBAC_H
#define GAME_UNK_OV005_0236BBAC_H

#include "types.h"

/* Overlay 5 state reached through data_ov005_0236bbac. */
typedef struct Unk_ov005_0236bbac {
    u8 pad_00[0x34];
    u32 unk_34;
    int coId;                          /* CO whose picture is shown; -1: none */
    u8 pad_3c[0x50 - 0x3c];
    void *unk_50;                      /* passed to Task_Destroy (task system) */
} Unk_ov005_0236bbac;

#endif
