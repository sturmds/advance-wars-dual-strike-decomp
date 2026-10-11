#ifndef GAME_EX_MACHINA_BOLT_TASK_H
#define GAME_EX_MACHINA_BOLT_TASK_H

#include "types.h"

/* Child task of gExMachinaScript (Von Bolt's Super CO Power) that runs the
 * script data_02168ad0 on the tile Strike_FindBestSite picked; created by
 * func_020e46cc. The first 0x2c bytes belong to the task system. */
typedef struct ExMachinaBoltTask {
    u8 pad_00[0x2c];
    int tileX;
    int tileY;
    u8 pad_34[0x64 - 0x34];
    s16 frame;                         /* frame counter of the blend steps */
} ExMachinaBoltTask;

#endif
