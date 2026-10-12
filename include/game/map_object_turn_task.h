#ifndef GAME_MAP_OBJECT_TURN_TASK_H
#define GAME_MAP_OBJECT_TURN_TASK_H

#include "types.h"
#include "game/map_object.h"

/* Task of script data_02168030 (Game_State13_SlidePanelStartTask starts
 * it): walks the map objects and lets each volcano and cannon act. The
 * first 0x2c bytes belong to the task system. */
typedef struct MapObjectTurnTask {
  u8 pad_00[0x2c];
  MapObject* nextObject; /* starts at Unk_02080440_48.objects */
} MapObjectTurnTask;

/* Child task of script data_021680e0 that MapObjectTurnTask starts for each
 * acting object: shows the shot for each object entry at the head of the
 * AI target list. */
typedef struct MapObjectFireTask {
  u8 pad_00[0x4c];
  s16 nextEntry; /* index in the AI target list (data_022a6dcc) */
} MapObjectFireTask;

#endif
