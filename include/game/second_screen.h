#ifndef GAME_SECOND_SCREEN_H
#define GAME_SECOND_SCREEN_H

#include "types.h"

/* Terrain panel on the second screen (ov000). The drawn values are copied
 * from the requested ones when SecondScreenUnit_IsStale finds a difference. */
extern int data_ov000_02350530; /* drawn: terrain or unit code (bits 0-4) */
extern int data_ov000_02350534; /* requested: owner */
extern int data_ov000_02350538; /* requested: terrain or unit code */
extern int data_ov000_0235053c; /* drawn: army style of the owner */
extern int data_ov000_02350540;
extern int data_ov000_02350544; /* requested: army style of the owner */
extern int data_ov000_02350548;
extern int data_ov000_0235054c; /* drawn: owner */

#endif
