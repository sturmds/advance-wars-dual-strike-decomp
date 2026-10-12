#ifndef GAME_TEXT_PRINTER_H
#define GAME_TEXT_PRINTER_H

#include "types.h"

/* One 0x60-byte text printing slot, set up by Text_PrintImmediate. It writes a
 * string from Text_Get into a 32-tile-wide BG screen map. */
typedef struct TextPrinter {
  u8 pad_00[0x20];
  const void* text;       /* 0x20: from Text_Get */
  const void* resumeText; /* 0x24: continued after text ends; NULL: none */
  u8* tiles;              /* 0x28: glyph tile data */
  u16* screen;            /* 0x2c: BG screen map, 32 entries per row */
  u16 unk_30;
  u16 unk_32; /* 0 when printing starts */
  u8 startX;  /* 0x34: in tiles */
  u8 startY;  /* 0x35: in tiles */
  u8 cursorX; /* 0x36: in tiles */
  u8 cursorY; /* 0x37: in tiles */
  u16 unk_38;
  u16 unk_3a;      /* same value as unk_38 when set up */
  u8 charSound;    /* 0x3c: nonzero plays a sound per character */
  s8 frameCounter; /* 0x3d: counts up to charDelay; negative: print at once */
  s8 charDelay;    /* 0x3e: frames per character */
  u8 pad_3f[0x40 - 0x3f];
  void*
      copyCallback; /* 0x40: queues the map copy; from GfxLayer_GetCopyCallback(0) */
  u8 pixelInTile; /* 0x44: pixel offset inside the cursor tile */
  u8 pad_45[0x46 - 0x45];
  u16 charsDrawn; /* 0x46 */
  u8 pad_48[0x60 - 0x48];
} TextPrinter;

#endif
