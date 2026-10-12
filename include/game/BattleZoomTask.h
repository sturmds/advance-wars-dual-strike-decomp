#ifndef GAME_BATTLEZOOMTASK_H
#define GAME_BATTLEZOOMTASK_H

#include "types.h"

/* Task of the ov002 battle animation made by func_ov002_0235cd64 from the script at
 * data_ov002_02369684. Each frame it zooms a sprite in at each side and, while shake is
 * set, draws a shaking sprite in the middle of the screen. The first 0x2c bytes belong to
 * the task system. */
typedef struct BattleZoomTask {
  u8 pad_00[0x4c];
  s16 frame;     /* frames since the start; the zoom takes 8 */
  s16 shakeStep; /* 0-8, one step of the shake pattern per frame */
  s16 shake; /* nonzero: draw the shaking sprite; set by func_ov002_0235cd64 */
  u16 shakeSize; /* pixels the sprite moves at each shake step */
  u8 pad_54[0x58 - 0x54];
  u32 paletteStep; /* index into BattleScreenWork.unk_fd0, taken mod 16 */
  u8 pad_5c[0x64 - 0x5c];
  s16 paletteSlot; /* palette colour that gets the cycled colour */
} BattleZoomTask;

#endif
