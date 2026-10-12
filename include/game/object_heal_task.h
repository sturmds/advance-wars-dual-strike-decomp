#ifndef GAME_OBJECT_HEAL_TASK_H
#define GAME_OBJECT_HEAL_TASK_H

#include "types.h"

/* Task that heals Black Hole units around a Black Crystal or a Black
 * Obelisk (script data_02167ff0; ObjectHealScan_StartNext starts one per standing
 * object). The first 0x2c bytes belong to the task system. */
typedef struct ObjectHealTask {
  u8 pad_00[0x2c];
  int tileX; /* tile the offsets count from */
  int tileY;
  u8 pad_34[0x54 - 0x34];
  int offsetList; /* list for Map_AddTileOffset: tiles in reach */
  int objectKind; /* MAP_OBJECT_BLACK_CRYSTAL or MAP_OBJECT_BLACK_OBELISK */
  int healSteps;  /* shown HP given to each unit (Unit_Repair) */
  u8 pad_60[0x64 - 0x60];
  s16 offsetIndex; /* next entry of offsetList */
} ObjectHealTask;

/* Task of script data_02167fb8: starts one ObjectHealTask after another,
 * for each standing Black Crystal and Black Obelisk on the map. */
typedef struct ObjectHealScanTask {
  u8 pad_00[0x2c];
  int nextObject; /* index in Unk_02080440_48.objects */
} ObjectHealScanTask;

#endif
