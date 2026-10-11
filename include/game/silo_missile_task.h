#ifndef GAME_SILO_MISSILE_TASK_H
#define GAME_SILO_MISSILE_TASK_H

#include "types.h"
#include "game/Unk_02022404.h"

/* Work area of the two silo missile tasks: the launch (script data_02168e48,
 * SiloMissileLaunch_Init and _Update) and the fall onto the target (script
 * data_02168e78, SiloMissileFall_Init and _Update, then SiloMissile_Explode). The
 * first 0x2c bytes belong to the task system. */
typedef struct SiloMissileTask {
    u8 pad_00[0x2c];
    int x;                             /* tile; the launch task turns it into map pixels (x * 16 + 8) */
    int y;                             /* tile; the launch task turns it into map pixels (y * 16 + 16) */
    u8 pad_34[0x44 - 0x34];
    s16 unk_44;                        /* fall task: read by SiloMissile_Explode */
    u8 pad_46[0x4a - 0x46];
    u16 spriteAttr;                    /* sprite tile | palette << 12 (the sprite's tileBase) */
    u8 pad_4c[0x50 - 0x4c];
    Unk_02022404 *sprite;
    int pixelX;                        /* fall task: map pixels of the target, x * 16 + 8 */
    int pixelY;                        /* y * 16 + 16 */
    u8 pad_5c[0x60 - 0x5c];
    int unk_60;                        /* fall task: read by SiloMissile_Explode */
    u8 pad_64[0x66 - 0x64];
    s16 fallY;                         /* fall task: map pixel row of the missile; starts at the view's top and grows by 8 a frame */
} SiloMissileTask;

#endif
