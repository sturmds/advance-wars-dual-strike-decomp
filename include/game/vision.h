#ifndef GAME_VISION_H
#define GAME_VISION_H

#include "types.h"
#include "game/map_unit.h"

/* Fog of war. Each map state has two vision grids, 0x508 bytes each, counted
 * from MapTiles.inSight: grid number Unk_02080440_48.unk_20 and unk_22.
 * A tile's byte counts the units and properties that see it. A player's
 * Player.unk_1b picks the grids its sight goes into: bit 0 the unk_20 grid,
 * bit 1 the unk_22 grid. */

/* Vision grid number n of a map state (Unk_02080440_48 *). */
#define VISION_GRID(state, n) ((state)->mapTiles->inSight + (n) * sizeof((state)->mapTiles->inSight))

void Map_RebuildVision(void);
void Player_AddVision(int player);
void Unit_AddVision(MapUnit *unit, s8 delta);
void Vision_AddArea(s16 x, s16 y, s16 range, int gridMask, s8 delta, int player);

#endif
