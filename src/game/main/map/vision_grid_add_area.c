#include "types.h"
#include "game/Unk_02080440.h"
#include "game/co_data.h"
#include "game/map_unit.h"
#include "game/skills.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/vision.h"

int abs(int value);
BOOL Unit_IsVisibleToActivePlayers(MapUnit* unit, s16 x, s16 y);

#define ABS(n) ((n) < 0 ? -(n) : (n))

/* Adds delta to each tile of grid within range steps of (x, y). Woods and
 * reefs more than one step away count only while a plane or copter is on them
 * (a dived Stealth only when Unit_IsVisibleToActivePlayers says so), unless
 * player has Piercing Vision. */
void VisionGrid_AddArea(s16 x, s16 y, s16 range, s8* grid, s8 delta,
                        u8 player) {
  s16 tx;
  s16 ty;
  s16 dy;
  u8 slot;
  MapUnit* unit;
  int terrain;
  int unitClass;

  if (range == 0) {
    grid[x + MAP_TILES->rowStart[y]] += delta;
    return;
  }
  for (ty = y - range, dy = -range; ty <= y + range; ty++, dy++) {
    if (ty < 0 || ty >= MAP_STATE->unk_1c4->mapHeight) {
      continue;
    }
    for (tx = x - range + abs(dy); tx <= x + range - abs(dy); tx++) {
      if (tx < 0 || tx >= MAP_STATE->unk_1c4->mapWidth) {
        continue;
      }
      if (!CheckSkillsEquipped(MAP_STATE->players[player].skills[0],
                               SKILL_PIERCING_VISION) &&
          ABS(tx - x) + ABS(ty - y) > 1) {
        terrain =
            MAP_TILE_TERRAIN(MAP_TILES->tiles[tx + MAP_TILES->rowStart[ty]]);
        if (terrain == TERRAIN_WOOD || terrain == TERRAIN_REEF) {
          slot = MAP_TILES->unk_508[tx + MAP_TILES->rowStart[ty]];
          if (slot == 0) {
            continue;
          }
          unit = &MAP_STATE->units[slot];
          unitClass = Ov000_gUnitStats[unit->type].unitClass;
          if (unitClass != UNIT_CLASS_PLANE && unitClass != UNIT_CLASS_COPTER) {
            continue;
          }
          if (unit->type == UNIT_STEALTH && (unit->flags & MAP_UNIT_DIVED) &&
              !Unit_IsVisibleToActivePlayers(unit, unit->x, unit->y)) {
            continue;
          }
        }
      }
      /* needed to match: the cast puts the tile value first in the add */
      grid[tx + MAP_TILES->rowStart[ty]] += (int)delta;
    }
  }
}
