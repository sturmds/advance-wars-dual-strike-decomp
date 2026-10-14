#ifndef GAME_MAP_POWER_LINE_H
#define GAME_MAP_POWER_LINE_H

#include "types.h"

/* Campaign map lines: a CO power used on map (mapId - 224) starts the script
 * when the CO and the number of power uses match; player 0 means any player.
 * A map's list ends with co -1. */
typedef struct MapPowerLine {
  u8 player;
  s8 co;
  u8 powerUses;
  u8 pad_03;
  u32 script;
} MapPowerLine;

typedef char MapPowerLine_size_check[sizeof(MapPowerLine) == 8 ? 1 : -1];

/* The list of each campaign map, by mapId - 224; NULL for none. */
extern MapPowerLine* data_02166f9c[];

#endif
