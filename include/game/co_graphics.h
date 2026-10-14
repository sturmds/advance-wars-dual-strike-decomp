#ifndef GAME_CO_GRAPHICS_H
#define GAME_CO_GRAPHICS_H

#include "types.h"

/* One picture variant of a CO: file names under /syogun. */
typedef struct CoPictureFiles {
  const char* name; /* loaded by Co_LoadGraphics */
  const char* unk_04;
} CoPictureFiles;

#define CO_PALETTE_COLOURS 16 /* colours in one of CoGraphics.palettes */

/* One entry of gCoGraphics (main), indexed by CO id. */
typedef struct CoGraphics {
  CoPictureFiles pictures[10]; /* indexed by the CO's stored picture variant */
  const u8* palettes; /* palettes of CO_PALETTE_COLOURS BGR555 colours */
} CoGraphics;

extern CoGraphics gCoGraphics[];

/* Bytes of a CO's small picture (12 tiles, Co_LoadSmallPicture), medium
 * picture (18 tiles, Co_QueueArmyMediumPicture) and mood picture (36 tiles,
 * Co_QueueLargePictureCopy), at 4 bits per pixel. */
#define CO_SMALL_PICTURE_SIZE 0x180
#define CO_MEDIUM_PICTURE_SIZE 0x240
#define CO_MOOD_PICTURE_SIZE 0x480

/* Bytes of a CO's large picture, as Co_LoadArmyGraphicsToBuffer writes it. */
#define CO_LARGE_PICTURE_SIZE 0x3600

#endif
