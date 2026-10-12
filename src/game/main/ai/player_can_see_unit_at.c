#include "types.h"
#include "game/skills.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Tile_HasTeammateUnit(
    u8 player, s16 x, s16 y); /* a unit of player's team stands on the tile */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* FALSE when a unit at (x, y) is hidden from `player` by cover: only with fog
 * or rain, on woods or reefs, for a player without Piercing Vision, when the
 * unit there is not an aircraft and no unit of the player's team stands on the
 * tile or next to it. Map vision itself is not checked. */
BOOL Player_CanSeeUnitAt(u16 player, s16 x, s16 y) {
  u16 count = 0;
  int terrain;

  if (STATE->unk_1c4->fog == 0 && STATE->unk_1c4->weather != WEATHER_RAIN) {
    return TRUE;
  }
  terrain = MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]);
  if (terrain != TERRAIN_WOOD && terrain != TERRAIN_REEF) {
    return TRUE;
  }
  if (CheckSkillsEquipped(STATE->players[player].skills[0],
                          SKILL_PIERCING_VISION)) {
    return TRUE;
  }
  if (Tile_HasTeammateUnit(player, x, y) == TRUE) {
    return TRUE;
  }
  if (Ov000_gUnitStats[STATE->units[STATE->mapTiles->unk_508[TILE(x, y)]].type]
          .moveGroup == MOVE_GROUP_AIR) {
    return TRUE;
  }
  if (x > 0) {
    count += Tile_HasTeammateUnit(player, x - 1, y);
  }
  if (y > 0) {
    count += Tile_HasTeammateUnit(player, x, y - 1);
  }
  if (x < STATE->unk_1c4->mapWidth - 1) {
    count += Tile_HasTeammateUnit(player, x + 1, y);
  }
  if (y < STATE->unk_1c4->mapHeight - 1) {
    count += Tile_HasTeammateUnit(player, x, y + 1);
  }
  return count != 0;
}
