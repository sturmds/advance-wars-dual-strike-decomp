#ifndef GAME_CO_SELECT_SCREEN_H
#define GAME_CO_SELECT_SCREEN_H

#include "types.h"

/* Work area of the CO select screen (Campaign, War Room, Survival),
 * through data_0229c73c. The COs sit in a grid; a cursor holds the column
 * in bits 1 and up and the row (0 or 1) in bit 0. */
typedef struct CoSelectScreen {
    u8 pad_00[0x35];
    u8 columnCount;
    u8 rowCount;                       /* up and down change row only when this is 2 */
} CoSelectScreen;

extern CoSelectScreen *data_0229c73c;

#endif
