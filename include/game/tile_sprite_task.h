#ifndef GAME_TILE_SPRITE_TASK_H
#define GAME_TILE_SPRITE_TASK_H

#include "types.h"

struct SpriteAnim;

/* Task TileSpriteTask_Start starts (scripts data_02166ed4 and data_02166efc) to
 * show an animated sprite on a map tile. The first 0x29 bytes belong to
 * the task system (Task_Create). */
typedef struct TileSpriteTask {
    u8 pad_00[0x29];
    u8 x;                              /* tile */
    u8 y;
    u8 pad_2b;
    u16 tile;                          /* first sprite tile of the current animation frame */
    u16 timer;                         /* frames since the animation started */
    u16 unk_30;
    u8 pad_32[0x34 - 0x32];
    struct SpriteAnim *sprite;       /* made by TileSpriteTask_CreateSprite */
} TileSpriteTask;

#endif
