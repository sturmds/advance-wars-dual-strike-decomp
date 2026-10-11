#ifndef GAME_MAP_OBJECT_TURN_TASK_H
#define GAME_MAP_OBJECT_TURN_TASK_H

#include "types.h"
#include "game/map_object.h"

/* Task of script data_02168030 (Game_State13_SlidePanelStartTask starts
 * it): walks the map objects and lets each volcano and cannon act. The
 * first 0x2c bytes belong to the task system. */
typedef struct MapObjectTurnTask {
    u8 pad_00[0x2c];
    MapObject *nextObject;             /* starts at Unk_02080440_48.objects */
} MapObjectTurnTask;

#endif
