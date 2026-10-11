#ifndef GAME_TILE_STRIKE_TASK_H
#define GAME_TILE_STRIKE_TASK_H

#include "types.h"
struct SpriteAnim;

/* Task that drops a sprite onto a map tile and shakes the screen when it
 * lands (scripts data_02167ef0 and data_02167f20, started by
 * TileStrike_Start). The first 0x2c bytes belong to the task system. The
 * sub screen draws it as a 2D sprite and the main screen in 3D; the same
 * fields serve both. */
typedef struct TileStrikeTask {
    u8 pad_00[0x2c];
    int tileX;
    int tileY;
    u8 pad_34[0x50 - 0x34];
    struct SpriteAnim *sprite;              /* handle from SpriteAnim_Create or BmapSpriteAnim_Create */
    int x;                             /* pixels on the map */
    int y;                             /* 2D: sprite y, moves down; 3D: tile y */
    int height;                        /* 3D only: counts down from 100 to 0 */
} TileStrikeTask;

/* Position of a 3D sprite, as SpriteAnim_Draw3d takes it. */
typedef struct TileStrikePos {
    int x;
    int y;
    int z;
} TileStrikePos;

#endif
