#ifndef GAME_VISION_H
#define GAME_VISION_H

#include "types.h"
#include "game/map_unit.h"

/* Fog of war. Each map state has two vision grids, 0x508 bytes each, counted
 * from MapTiles.inSight: grid numbers Unk_02080440_48.visionGrid0 and visionGrid1.
 * A tile's byte counts the units and properties that see it. A player's
 * Player.visionGrids picks the grids its sight goes into: bit 0 the visionGrid0 grid,
 * bit 1 the visionGrid1 grid. */

/* What Player_GetSlotRelation(a, b) returns: slot b has gSlotControllers[b] == 0 (none),
 * or is on another team than a (enemy) or the same team (ally). */
#define SLOT_RELATION_NONE 0
#define SLOT_RELATION_ENEMY 1
#define SLOT_RELATION_ALLY 2

/* Vision grid number n of a map state (Unk_02080440_48 *). */
#define VISION_GRID(state, n) \
  ((state)->mapTiles->inSight + (n) * sizeof((state)->mapTiles->inSight))

void Map_RebuildVision(void);
void Player_AddVision(int player);
void Unit_AddVision(MapUnit* unit, s8 delta);
void Vision_AddArea(s16 x, s16 y, s16 range, int gridMask, s8 delta,
                    int player);
void VisionGrid_AddArea(s16 x, s16 y, s16 range, s8* grid, s8 delta, u8 player);

#endif
