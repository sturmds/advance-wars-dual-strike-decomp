#ifndef GAME_PLAYER_INFO_TASK_H
#define GAME_PLAYER_INFO_TASK_H

#include "types.h"

/* Task of the player info screen script (data_0215d430), started by
 * PlayerInfo_StartTaskFor and PlayerInfo_StartTaskForCurrentPlayer. The
 * first 0x64 bytes belong to the task system. */
typedef struct PlayerInfoTask {
  u8 pad_00[0x64];
  u16 player; /* player whose info the screen opens on */
  u16 unk_66; /* 1 when the player was read from the other screen's state */
  u16 unk_68; /* 1 when the caller passed player and unk_66, 0 when they come from the game state */
} PlayerInfoTask;

/* Task of the script that draws the player info screen's sprites (the one
 * whose callbacks are PlayerInfo_SpriteTaskInit and PlayerInfo_DrawSprites).
 * The first 0x2a bytes belong to the task system. */
typedef struct PlayerInfoSpriteTask {
  u8 pad_00[0x2a];
  u16 slideFrame;  /* frames of the current slide, 0-8; 8 at start */
  u16 waitFrames;  /* frames waited off screen before the new CO slides in */
  s16 slideStartX; /* x offset of the portrait when the slide started */
  s16 slideX;      /* current x offset of the portrait, 0-144 */
  u8 coButton; /* (player - 1) * 2 + slot: the id of the shown CO's button */
  u8 prevCoButton; /* same at start */
  u8 slidingIn;    /* 1 at start; 0 while the old CO slides out */
} PlayerInfoSpriteTask;

#endif
