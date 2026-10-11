#ifndef GAME_EVENT_PICTURE_H
#define GAME_EVENT_PICTURE_H

#include "types.h"
#include "game/Unk_022977a4.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

/* The CO picture that event scripts show next to their text: a 6 by 6 tile
 * block in the layer tile map buffer gEventTextMap. */

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define EVENT_PICTURE_SIZE      6     /* tiles per side */
#define EVENT_PICTURE_TILE_BASE 0x1b3 /* first tile of the picture, stored by rows */
#define EVENT_PICTURE_PALETTE   0x1000
#define EVENT_PICTURE_RIGHT_X   31    /* last column of the 32-tile map */
#define EVENT_PICTURE_MAP_WIDTH 32
#define EVENT_PICTURE_HFLIP     0x400 /* tile map entry: flip horizontally */

/* In the campaign, a Black Hole speaker's picture sits at the right edge, mirrored. */
#define EVENT_PICTURE_ON_RIGHT                                                          \
    (data_027c027c->unk_48[data_027c0284]->gameMode == GAME_MODE_CAMPAIGN             \
     && data_022977a4.armyColour == ARMY_BLACK_HOLE)

/* Map column of the picture's first column. */
static inline int EventPicture_GetX(void)
{
    return EVENT_PICTURE_ON_RIGHT ? EVENT_PICTURE_RIGHT_X : 0;
}

#endif
