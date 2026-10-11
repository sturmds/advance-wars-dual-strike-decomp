#ifndef GAME_MISSILE_STRIKE_TASK_H
#define GAME_MISSILE_STRIKE_TASK_H

#include "types.h"

/* Work area of the missile strike task (script data_02169088), started on
 * list 3 by func_020eb9dc. The first 0x34 bytes belong to the task system. */
typedef struct MissileStrikeTask {
    u8 pad_00[0x34];
    int tileX;                         /* tile the strike lands on */
    int tileY;
    int unk_3c;                        /* set to 8 */
    u8 pad_40[0x54 - 0x40];
    int unk_54;                        /* third argument of func_020eb9dc (2 from func_020f1f38) */
    int unk_58;                        /* fourth argument (80 from func_020f1f38) */
} MissileStrikeTask;

#endif
