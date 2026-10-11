#ifndef GAME_UNK_OV005_0236BBA4_H
#define GAME_UNK_OV005_0236BBA4_H

#include "types.h"

struct Unk_ov005_0236bba4;

/* One state of the screen's state table, 20 bytes. Ov005_RunState
 * calls run(work, arg). */
typedef struct Unk_ov005_0236bba4_State {
    u32 unk_00;                        /* nonzero: the state is in use (Ov005_StateIsEnabled) */
    int number;                        /* shown on the main screen as two digits */
    int scrollMax;                     /* fx32 rows; the scroll is clamped to 0..scrollMax */
    void (*run)(struct Unk_ov005_0236bba4 *work, int arg);
    int arg;
} Unk_ov005_0236bba4_State;

/* Work block (0x39c bytes) that Ov005_AllocStateWork allocates into
 * Ov005Task.work and data_ov005_0236bba4. */
typedef struct Unk_ov005_0236bba4 {
    u32 rTouched;                      /* 1: the screen acts as if R were pressed (Ov005StateScreen_OnRTouched) */
    u32 bTouched;                      /* 1: the screen acts as if B were pressed */
    u32 prevTouched;                   /* 1: acts as Left (previous state) */
    u32 nextTouched;                   /* 1: acts as Right (next state) */
    u32 leftTouched;                   /* set while the Left button is pressed or held; the screen then acts as if the Left key were down */
    u32 rightTouched;                  /* same for the Right button */
    u32 state;                         /* index into stateTable */
    u32 unk_1c;
    u32 unk_20;
    u32 unk_24;
    u32 unk_28;
    u32 button245Shown;
    u32 button246Shown;
    u32 unk_34;
    u32 unk_38;                        /* GameFlag 61 */
    u32 unk_3c;                        /* GameFlag 62 */
    u32 unk_40;                        /* GameFlag 34 */
    u32 unk_44;                        /* GameFlag 64 */
    u32 unk_48;                        /* GameFlag 65 */
    u32 unk_4c;
    u32 unk_50;
    Unk_ov005_0236bba4_State states[41]; /* copy of data_ov005_0236a74c */
    Unk_ov005_0236bba4_State *stateTable; /* points at states */
    void *spriteFile;                  /* /rikiishi/050; SpriteAnim_Create makes the unk_390 sprites from it */
    void *unk_390[3];
} Unk_ov005_0236bba4;

#endif
