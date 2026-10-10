#ifndef GAME_UNK_OV005_0236BBA4_H
#define GAME_UNK_OV005_0236BBA4_H

#include "types.h"

struct Unk_ov005_0236bba4;

/* One state of the screen's state table, 20 bytes. Ov005_RunState
 * calls run(work, arg). */
typedef struct Unk_ov005_0236bba4_State {
    u32 unk_00;                        /* nonzero: the state is in use (Ov005_StateIsEnabled) */
    u8 pad_04[0xc - 0x4];
    void (*run)(struct Unk_ov005_0236bba4 *work, int arg);
    int arg;
} Unk_ov005_0236bba4_State;

/* Work block (0x39c bytes) that func_ov005_0235ea58 allocates into
 * Ov005Task.work and data_ov005_0236bba4. */
typedef struct Unk_ov005_0236bba4 {
    u8 pad_00[0x8];
    u32 unk_08;
    u32 unk_0c;
    u32 leftTouched;                   /* set while the Left button is pressed or held; the screen then acts as if the Left key were down */
    u32 rightTouched;                  /* same for the Right button */
    u32 state;                         /* index into stateTable */
    u8 pad_1c[0x2c - 0x1c];
    u32 unk_2c;                        /* nonzero: button 245 exists */
    u32 unk_30;                        /* nonzero: button 246 exists */
    u8 pad_34[0x38 - 0x34];
    u32 unk_38;                        /* GameFlag 61 */
    u32 unk_3c;                        /* GameFlag 62 */
    u32 unk_40;                        /* GameFlag 34 */
    u32 unk_44;                        /* GameFlag 64 */
    u32 unk_48;                        /* GameFlag 65 */
    u32 unk_4c;
    u32 unk_50;
    Unk_ov005_0236bba4_State states[41]; /* copy of data_ov005_0236a74c */
    Unk_ov005_0236bba4_State *stateTable; /* points at states */
    u8 pad_38c[0x390 - 0x38c];
    void *unk_390[3];
} Unk_ov005_0236bba4;

#endif
