#ifndef GAME_LAYER0_PICTURE_TASK_H
#define GAME_LAYER0_PICTURE_TASK_H

#include "types.h"

/* Task that loads a picture into BG layer 0: the first 0x4c bytes belong
 * to the task system (Task_Create). */
typedef struct Layer0PictureTask {
  u8 pad_00[0x4c];
  const char* mapFile; /* tile map of the picture */
  const char* paletteFile;
} Layer0PictureTask;

#endif
