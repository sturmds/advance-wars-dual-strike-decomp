#ifndef GAME_TEXT_PRINTER_H
#define GAME_TEXT_PRINTER_H

#include "types.h"

/* One 0x60-byte text printing slot, set up by Text_PrintImmediate. It writes a
 * string from Text_Get into a 32-tile-wide BG screen map. */
typedef struct TextPrinter {
    u8 pad_00[0x20];
    const void *text;                  /* 0x20: from Text_Get */
    const void *resumeText;            /* 0x24: continued after text ends; NULL: none */
    u8 pad_28[0x2c - 0x28];
    u16 *screen;                       /* 0x2c: BG screen map, 32 entries per row */
    u16 unk_30;
    u16 unk_32;                        /* 0 when printing starts */
    u8 startX;                         /* 0x34: in tiles */
    u8 pad_35[0x36 - 0x35];
    u8 cursorX;                        /* 0x36: in tiles */
    u8 cursorY;                        /* 0x37: in tiles */
    u8 pad_38[0x3d - 0x38];
    s8 unk_3d;
    s8 unk_3e;
    u8 pad_3f[0x60 - 0x3f];
} TextPrinter;

#endif
