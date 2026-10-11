#ifndef GAME_BLACK_BOMB_BLAST_TASK_H
#define GAME_BLACK_BOMB_BLAST_TASK_H

#include "types.h"

/* Work area of the Black Bomb blast task (script data_02168f10), a child of
 * the explosion task BlackBombExplosion_Start starts. */
typedef struct BlackBombBlastTask {
    u8 pad_00[0x2c];
    int tileX;                         /* centre of the blast */
    int tileY;
    u8 pad_34[0x44 - 0x34];
    s16 damage;                        /* hp taken off each unit in range */
    u8 pad_46[0x4a - 0x46];
    u16 sprite;                        /* bits 0-11 graphics tile, bits 12-15 palette */
    u8 pad_4c[0x54 - 0x4c];
    int pixelX;                        /* centre of tileX in map pixels */
    int pixelY;
    u8 pad_5c[0x66 - 0x5c];
    s16 scrollY;                       /* BlackBombBlast_InitPosition copies Unk_02080440_1c4.scrollY here */
} BlackBombBlastTask;

/* Work area of the explosion task BlackBombExplosion_Start starts (script
 * data_02168f90). */
typedef struct BlackBombExplosionTask {
    u8 pad_00[0x2c];
    int tileX;                         /* tile the bomb is exploding on */
    int tileY;
    u8 pad_34[0x4a - 0x34];
    u16 damage;                        /* 50, passed on to the blast task */
    u8 pad_4c[0x64 - 0x4c];
    s16 bombX;                         /* tile of the bomb (the first two arguments of BlackBombExplosion_Start) */
    s16 bombY;
} BlackBombExplosionTask;

#endif
