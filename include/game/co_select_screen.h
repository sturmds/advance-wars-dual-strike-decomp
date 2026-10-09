#ifndef GAME_CO_SELECT_SCREEN_H
#define GAME_CO_SELECT_SCREEN_H

#include "types.h"

/* Work area of the CO select screen (Campaign, War Room, Survival),
 * through data_0229c73c. The COs sit in a grid; a cursor holds the column
 * in bits 1 and up and the row (0 or 1) in bit 0. */
typedef struct CoSelectScreen {
    u8 coGrid[0x2c];                   /* CO id per cell at (cursorColumn - 1) * 2 + cursorRow; 0: empty */
    u8 *unk_2c;                        /* byte table read by func_020457d8 */
    u8 pad_30;
    u8 cursorColumn;                   /* counts from 1 */
    u8 cursorRow;
    u8 pad_33[0x35 - 0x33];
    u8 columnCount;
    u8 rowCount;                       /* up and down change row only when this is 2 */
} CoSelectScreen;

extern CoSelectScreen *data_0229c73c;

#endif
