#ifndef GAME_OV003_GFX_H
#define GAME_OV003_GFX_H

#include "types.h"
#include "game/ov003_unit.h"

/* Overlay 3 graphics buffers. */
#define OV003_TEXT_TILES_SIZE 0x2000
extern void* data_ov003_02365b0c; /* text tiles for OBJ VRAM */
extern u16 data_ov003_02369be8[]; /* colours cycled into the palette */

/* CO pictures (CO_LARGE_PICTURE_SIZE bytes each) of the team rows: the
 * first rows' in gMenuState, the others' in data_ov003_02365b10. */
#define OV003_PICTURES_IN_MENU_STATE 2
#define OV003_PICTURES_ALLOCATED \
  (OV003_TEAM_ROW_COUNT - OV003_PICTURES_IN_MENU_STATE)

/* Map entries of the 2 x 2 tiles of each 16x16 metatile, unpacked from two
 * maps of 32 x 16 metatiles by Ov003_LoadGraphics. */
#define OV003_METATILE_COUNT (32 * 16)
extern u16 data_ov003_02367110[OV003_METATILE_COUNT][4]; /* layers 2 and 3 */
extern u16 data_ov003_02366110[OV003_METATILE_COUNT][4]; /* layer 1 */

#endif
