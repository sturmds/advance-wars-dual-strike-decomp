#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/skills.h"
#include "game/terrain.h"
#include "game/unit_stats.h"
#include "game/ai/ai_best_damage.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* a unit of player's team stands on the tile */
#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* Chart damage with the player's firepower: (player, slot, powerState, unitType, targetType, weapon, targetDived). */
int Co_GetSlotChartDamage(int player, int slot, int powerState, int unitType,
                          int targetType, int weapon, BOOL targetDived);
BOOL Tile_HasTeammateUnit(u8 player, s16 x, s16 y);

/* FALSE when a unit at (x, y) is hidden from `player` by cover: only with fog
 * or rain, on woods or reefs, for a player without Piercing Vision, when the
 * unit there is not an aircraft and no unit of the player's team stands on the
 * tile or next to it. Map vision itself is not checked. */
BOOL Player_CanSeeUnitAt(u16 player, s16 x, s16 y) {
  u16 count = 0;
  int terrain;

  if (MAP_STATE->unk_1c4->fog == 0 &&
      MAP_STATE->unk_1c4->weather != WEATHER_RAIN) {
    return TRUE;
  }
  terrain = MAP_TILE_TERRAIN(MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]);
  if (terrain != TERRAIN_WOOD && terrain != TERRAIN_REEF) {
    return TRUE;
  }
  if (CheckSkillsEquipped(MAP_STATE->players[player].skills[0],
                          SKILL_PIERCING_VISION)) {
    return TRUE;
  }
  if (Tile_HasTeammateUnit(player, x, y) == TRUE) {
    return TRUE;
  }
  if (Ov000_gUnitStats
          [MAP_STATE->units[MAP_STATE->mapTiles->unk_508[MAP_TILE_INDEX(x, y)]]
               .type]
              .moveGroup == MOVE_GROUP_AIR) {
    return TRUE;
  }
  if (x > 0) {
    count += Tile_HasTeammateUnit(player, x - 1, y);
  }
  if (y > 0) {
    count += Tile_HasTeammateUnit(player, x, y - 1);
  }
  if (x < MAP_STATE->unk_1c4->mapWidth - 1) {
    count += Tile_HasTeammateUnit(player, x + 1, y);
  }
  if (y < MAP_STATE->unk_1c4->mapHeight - 1) {
    count += Tile_HasTeammateUnit(player, x, y + 1);
  }
  return count != 0;
}

/* Best damage of a unit type's two weapons against a target type that has
 * not dived, using the current player's front CO. */
int Ai_BestDamage(int unitType, int targetType, int powerState) {
  int primary = Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 0, powerState,
                                      unitType, targetType, 0, FALSE);
  int secondary = Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 0, powerState,
                                        unitType, targetType, 1, FALSE);

  if (primary <= secondary) {
    primary = secondary;
  }
  return primary;
}

/* Best damage of a unit type's two weapons against a dived target type (a
 * Sub or Stealth), using the current player's firepower. */
int Ai_BestDamageVsDived(int unitType, int targetType, int powerState) {
  int primary = Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 0, powerState,
                                      unitType, targetType, 0, TRUE);
  int secondary = Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 0, powerState,
                                        unitType, targetType, 1, TRUE);

  if (primary <= secondary) {
    primary = secondary;
  }
  return primary;
}

/* Best damage of a unit type's two weapons against a target type that has
 * not dived, for the current player. With a back CO, each weapon's damage
 * is the average of the front and back CO's. */
int Ai_CoPairBestDamage(int unitType, int targetType, int powerState) {
  int primary = Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 0, powerState,
                                      unitType, targetType, 0, FALSE);
  int secondary = Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 0, powerState,
                                        unitType, targetType, 1, FALSE);
  BOOL hasBackCo =
      MAP_STATE->players[MAP_STATE->currentPlayer].back.id ? TRUE : FALSE;

  if (hasBackCo == TRUE) {
    primary += Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 1, powerState,
                                     unitType, targetType, 0, FALSE);
    secondary += Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 1, powerState,
                                       unitType, targetType, 1, FALSE);
    primary >>= 1;
    secondary >>= 1;
  }
  if (primary > secondary) {
    return primary;
  }
  return secondary;
}

/* Best damage of a unit type's two weapons against a dived target type,
 * for the current player. With a back CO, each weapon's damage is the
 * average of the front and back CO's. */
int Ai_CoPairBestDamageVsDived(int unitType, int targetType, int powerState) {
  int primary = Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 0, powerState,
                                      unitType, targetType, 0, TRUE);
  int secondary = Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 0, powerState,
                                        unitType, targetType, 1, TRUE);
  BOOL hasBackCo =
      MAP_STATE->players[MAP_STATE->currentPlayer].back.id ? TRUE : FALSE;

  if (hasBackCo == TRUE) {
    primary += Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 1, powerState,
                                     unitType, targetType, 0, TRUE);
    secondary += Co_GetSlotChartDamage(MAP_STATE->currentPlayer, 1, powerState,
                                       unitType, targetType, 1, TRUE);
    primary >>= 1;
    secondary >>= 1;
  }
  if (primary > secondary) {
    return primary;
  }
  return secondary;
}
