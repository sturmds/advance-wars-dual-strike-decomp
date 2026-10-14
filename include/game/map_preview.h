#ifndef GAME_MAP_PREVIEW_H
#define GAME_MAP_PREVIEW_H

#include "types.h"

/* State of the map preview that the menus draw (MapPreview_*). */

/* 0 while the main loop runs MainLoop_MapFrame: then the halves skip the
 * neutral colour check */
extern int data_0229b054;
/* index of the first tile of the row the halves draw next */
extern int data_0229b058;
/* property counts the last MapPreview_Draw left */
extern u8 gMapPreviewPropertyCounts[];
extern u32 data_022a6b3c; /* palette row of the preview */

#endif
