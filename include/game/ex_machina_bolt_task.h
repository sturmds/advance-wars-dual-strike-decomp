#ifndef GAME_EX_MACHINA_BOLT_TASK_H
#define GAME_EX_MACHINA_BOLT_TASK_H

#include "types.h"

/* Child task of gExMachinaScript (Von Bolt's Super CO Power) that runs the
 * script gExMachinaBoltScript on the tile Strike_FindBestSite picked; created by
 * ExMachinaBolt_Start. The first 0x2c bytes belong to the task system. */
typedef struct ExMachinaBoltTask {
  u8 pad_00[0x2c];
  int tileX;
  int tileY;
  u8 pad_34[0x64 - 0x34];
  s16 frame; /* frame counter of the blend steps */
} ExMachinaBoltTask;

/* The task that runs gExMachinaScript. The first 0x66 bytes belong to the
 * task system and to steps that are not decompiled yet. */
typedef struct ExMachinaTask {
  u8 pad_00[0x66];
  s16 targetX; /* tile Strike_FindBestSite picked */
  s16 targetY;
} ExMachinaTask;

#endif
