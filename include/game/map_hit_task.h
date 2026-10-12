#ifndef GAME_MAP_HIT_TASK_H
#define GAME_MAP_HIT_TASK_H

#include "types.h"
#include "game/map_object.h"

/* Task (script data_02168d68) that shows a hit on a map object or on a pipe
 * seam tile; MapHitTask_ShowResult runs it each frame. A child of the attack's
 * task. The first 0x2c bytes belong to the task system. */
typedef struct MapHitTask {
  u8 pad_00[0x2c];
  int x; /* tile */
  int y;
  u8 pad_34[0x4c - 0x34];
  MapObject* object; /* NULL for a pipe seam tile */
  u8 pad_50[0x64 - 0x50];
  s16 kind; /* MapObject.kind, or 6 for a pipe seam */
  s16 hp;   /* hp left; 0 or less: the object or seam is destroyed */
} MapHitTask;

/* MapHitTask.kind of a pipe seam tile (it has no MapObject). */
#define MAP_HIT_PIPE_SEAM 6

#endif
