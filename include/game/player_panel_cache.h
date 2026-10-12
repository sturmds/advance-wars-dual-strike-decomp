#ifndef GAME_PLAYER_PANEL_CACHE_H
#define GAME_PLAYER_PANEL_CACHE_H

#include "types.h"

/* What the second-screen player rows last showed, so they are redrawn only
 * when a value changes. Indexed by player slot; slot 0 is unused. */
typedef struct PlayerPanelCache {
  int unitCount;
  int properties; /* -1 when fog or rain hides the count */
  int inGame;
} PlayerPanelCache;

extern PlayerPanelCache Ov000_gPlayerPanelCache[5];

#endif
