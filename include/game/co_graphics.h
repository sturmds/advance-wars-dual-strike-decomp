#ifndef GAME_CO_GRAPHICS_H
#define GAME_CO_GRAPHICS_H

#include "types.h"

/* One picture variant of a CO: file names under /syogun. */
typedef struct CoPictureFiles {
  const char* name; /* loaded by Co_LoadGraphics */
  const char* unk_04;
} CoPictureFiles;

/* One entry of gCoGraphics (main), indexed by CO id. */
typedef struct CoGraphics {
  CoPictureFiles pictures[10]; /* indexed by the CO's stored picture variant */
  const u8* palettes;          /* 16-colour palettes, 32 bytes each */
} CoGraphics;

extern CoGraphics gCoGraphics[];

/* Bytes of a CO's large picture, as Co_LoadArmyGraphicsToBuffer writes it. */
#define CO_LARGE_PICTURE_SIZE 0x3600

#endif
