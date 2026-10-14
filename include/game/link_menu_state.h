#ifndef GAME_LINK_MENU_STATE_H
#define GAME_LINK_MENU_STATE_H

#include "types.h"

/* State of the link play player list screen, pointed to by data_0215fed0. */
typedef struct LinkMenuState {
  u8 view; /* view drawn last: 0 player list, 1 second view (LinkMenu_DrawSecondView); Link_DrawPlayerRows redraws the layer 2 background when it was 1 */
  u8 selected; /* id of the selected button, 0-4 */
  u8 pad_02[2];
  int locked; /* 1 once a choice was made; key handlers then ignore input */
} LinkMenuState;

extern LinkMenuState* data_0215fed0;

extern LinkMenuState data_0229d88c; /* what data_0215fed0 points to */

#endif
