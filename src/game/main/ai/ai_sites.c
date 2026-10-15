#include "types.h"
#include "game/ai_site.h"
#include "game/ai_profile.h"
#include "game/ai_site_group.h"
#include "game/ai_unit_action.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/ai_build_state.h"
#include "game/map_unit.h"
#include "game/range_map.h"
#include "game/ai/ai_sites.h"
#include "game/ai/ai_build_buy.h"
#include "game/ai/ai_build_pick.h"
#include "game/ai/ai_site_pick.h"
#include "game/ai/ai_build_checks.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define TILE_TERRAIN(x, y) \
  MAP_TILE_TERRAIN(        \
      MAP_STATE->mapTiles->tiles[(x) + MAP_STATE->mapTiles->rowStart[y]])
#define FAR_AWAY 110
#define UNIT_OWNER(slot) (((slot) >> 6) + 1)

extern s8 data_0229e5c0[][12];
extern u8 gTerrainFactoryKind[];
extern u8 gUnitFitsLander[];

/* The kind byte of each AiSite record (data_0229e5bc + 3); the original
 * addresses it through its own symbol. */
extern u8 data_0229e5bf[][sizeof(AiSite)];
int Unit_GetValuePerHp(int playerIndex, int unitType);
/* per terrain id: 1 base, 2 airport, 3 port, else 0 */
BOOL Map_IsFreeDeployCity(int x, int y);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int Map_GetDistance(int x1, int y1, s16 x2, s16 y2);
BOOL Terrain_IsProperty(u8 tile);
int Unit_IsTileOwnedByTeam(int unitIndex, int tile);
BOOL Unit_IsHarmless(MapUnit* unit);
int GetUnitMaxRange(int player, int unitType);

/* Groups the AI sites: each site with no group yet starts one, and every site
 * an Infantry can reach from it (FAR_AWAY moves) joins it. Fills the group's
 * AiSiteGroup from the units and properties in that reach, lowers the first
 * site's propertyDistance, then hands the group to AiSiteGroup_RateSurroundings. Returns the
 * number of groups. */
int AiSites_BuildGroups(void) {
  /* needed to match: this declaration order */
  AiSite* site;
  AiSiteGroup* group;
  int result;
  int slot;
  AiSite* other;
  int j;
  MapUnit* unit;
  int i;
  u8 groundFlags;
  u8 airFlags;
  int x;
  int y;
  UnitStats* stats;
  int count;

  count = 0;
  site = data_0229e5bc;
  group = gAiSiteGroups;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->group != -1) {
      continue;
    }
    RangeMap_FillMoves(site->x, site->y, UNIT_INFANTRY, FAR_AWAY, 0);
    group->threat = 0;
    group->skipFoot = 0;
    group->needAntiAir = 0;
    group->propertyCount = 0;
    group->unk_01 = 0;
    groundFlags = 0;
    airFlags = 0;
    for (j = 0, other = data_0229e5bc; j < data_021619c0->recordCount;
         j++, other++) {
      if (MAP_STATE->rangeMap[other->y][other->x] >= 0) {
        other->group = count;
      }
    }
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
        if (slot != 0) {
          unit = &MAP_STATE->units[slot];
          if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
              MAP_STATE->players[(slot >> 6) + 1].team) {
            if (GetUnitMaxRange(MAP_STATE->currentPlayer, unit->type) != 0) {
              group->threat--;
            }
            if (unit->type <= UNIT_MECH && unit->hp >= 61) {
              group->skipFoot = 1;
            }
            if ((unit->type == UNIT_ANTI_AIR || unit->type == UNIT_FIGHTER) &&
                unit->hp >= 61) {
              airFlags |= 1;
            }
          } else if (!Unit_IsHarmless(unit)) {
            group->threat += 2;
            stats = &Ov000_gUnitStats[unit->type];
            if ((stats->moveGroup &
                 (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) &&
                unit->type > UNIT_MECH) {
              groundFlags |= 1;
            }
            if ((stats->moveGroup & MOVE_GROUP_AIR) &&
                unit->type != UNIT_T_COPTER) {
              airFlags |= 2;
            }
          }
        }
        if (Terrain_IsProperty(
                MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]) == 1 &&
            !Unit_IsTileOwnedByTeam(
                (s16)MAP_STATE->currentUnitBase,
                MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)])) {
          if (site->propertyMoves > MAP_STATE->rangeMap[y][x]) {
            site->propertyMoves = MAP_STATE->rangeMap[y][x];
          }
          groundFlags |= 2;
          group->propertyCount++;
        }
      }
    }
    if ((groundFlags & 1) || !(groundFlags & 2)) {
      group->skipFoot = 1;
    }
    if (!(airFlags & 1) && (airFlags & 2)) {
      group->needAntiAir = 1;
    }
    result = AiSite_RateLandmass(site->x, site->y);
    if ((result & 0x100) || !(result & 0xff)) {
      group->unk_03_2 = 0;
    } else {
      group->unk_03_2 = 1;
    }
    AiSiteGroup_RateSurroundings(count);
    count++;
    group++;
  }
  return count;
}

/* TRUE when a unit counts as able to reach the site. At a base, a ground unit
 * must stand on a tile the range map reaches and a ship must carry cargo; at
 * a port, ground units never count. Air units always do. */
BOOL AiSite_UnitCanReach(int siteIndex, MapUnit* unit) {
  u8 moveGroup;

  if (data_0229e5bc[siteIndex].type == SITE_BASE) {
    moveGroup = Ov000_gUnitStats[unit->type].moveGroup;
    if (moveGroup & (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
      if (MAP_STATE->rangeMap[unit->y][unit->x] < 0) {
        return FALSE;
      }
    }
    if (moveGroup & MOVE_GROUP_SEA) {
      if (unit->cargo[0] == 0) {
        return FALSE;
      }
    }
  }
  if (data_0229e5bc[siteIndex].type == SITE_PORT) {
    if (Ov000_gUnitStats[unit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
      return FALSE;
    }
  }
  return TRUE;
}

/* Rates the area around each AiSite of one group:
 * - enemyDistance: distance to the nearest enemy unit that AiSite_UnitCanReach lets
 *   count for the site.
 * - unitBalance, enemyCount: units within gAiProfile->unk_08[8] tiles that a
 *   Tank could reach, skipping unarmed units with no cargo (Unit_IsHarmless).
 * - propertyMoves (when still 0xff): Infantry move cost to the nearest
 *   property the current team does not own. */
void AiSiteGroup_RateSurroundings(int group) {
  AiSite* site;
  int maxX;
  int maxY;
  int dist;
  u8 range;
  int y;
  MapUnit* unit;
  int i;
  int minY;
  u8 slot;
  int x;
  int minX;
  int propertyX; /* needed for the register order: the last loop gets its own x */

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
      if (slot == 0) {
        continue;
      }
      unit = &MAP_STATE->units[slot];
      if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
          MAP_STATE->players[UNIT_OWNER(slot)].team) {
        continue;
      }
      site = data_0229e5bc;
      for (i = 0; i < data_021619c0->recordCount; i++, site++) {
        if (site->group != group) {
          continue;
        }
        if (!AiSite_UnitCanReach(i, unit)) {
          continue;
        }
        dist = Map_GetDistance(site->x, site->y, x, y);
        if (site->enemyDistance > dist) {
          site->enemyDistance = dist;
        }
      }
    }
  }

  site = data_0229e5bc;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->group != group) {
      continue;
    }
    RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeScratch);
    RangeMap_FillMoves(site->x, site->y, UNIT_TANK, gAiProfile->unk_08[8], 0);
    RangeMap_SetBuffer(MAP_STATE->mapTiles->rangeMapTiles);

    range = gAiProfile->unk_08[8];
    /* Needed to match: these clamp forms and the declaration order set
         * the spill slots and the order the hoisted zeros are stored in. */
    if (site->x - range < 0) {
      minX = 0;
    } else {
      minX = site->x - range;
    }
    if (site->y - range < 0) {
      minY = 0;
    } else {
      minY = site->y - range;
    }
    if (site->x + range >= MAP_STATE->unk_1c4->mapWidth - 1) {
      maxX = MAP_STATE->unk_1c4->mapWidth - 1;
    } else {
      maxX = site->x + range;
    }
    if (site->y + range >= MAP_STATE->unk_1c4->mapHeight - 1) {
      maxY = MAP_STATE->unk_1c4->mapHeight - 1;
    } else {
      maxY = site->y + range;
    }
    for (y = minY; y <= maxY; y++) {
      for (x = minX; x <= maxX; x++) {
        dist = 0;
        if (site->x - x > 0) {
          dist += site->x - x;
        } else {
          dist += x - site->x;
        }
        if (site->y - y > 0) {
          dist += site->y - y;
        } else {
          dist += y - site->y;
        }
        if (dist < 0 || dist > gAiProfile->unk_08[8]) {
          continue;
        }
        slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
        if (slot == 0) {
          continue;
        }
        unit = &MAP_STATE->units[slot];
        if (!AiSite_UnitCanReach(i, unit)) {
          continue;
        }
        if (Unit_IsHarmless(unit) == TRUE) {
          continue;
        }
        if ((Ov000_gUnitStats[unit->type].moveGroup &
             (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) &&
            MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(x, y)] < 0) {
          continue;
        }
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team ==
            MAP_STATE->players[UNIT_OWNER(slot)].team) {
          site->unitBalance--;
          continue;
        }
        if (site->type == SITE_PORT || unit->type <= UNIT_MECH) {
          site->unitBalance += 1;
        } else {
          site->unitBalance += 2;
        }
        site->enemyCount++;
      }
    }

    if (site->propertyMoves != 0xff) {
      continue;
    }
    RangeMap_FillMoves(site->x, site->y, UNIT_INFANTRY, FAR_AWAY, 0);
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (propertyX = 0; propertyX < MAP_STATE->unk_1c4->mapWidth;
           propertyX++) {
        if (MAP_STATE->rangeMap[y][propertyX] < 0) {
          continue;
        }
        if (Terrain_IsProperty(
                MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(propertyX, y)]) !=
            TRUE) {
          continue;
        }
        /* the original reads currentUnitBase with ldrsh */
        if (Unit_IsTileOwnedByTeam(
                (s16)MAP_STATE->currentUnitBase,
                MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(propertyX, y)])) {
          continue;
        }
        if (site->propertyMoves > MAP_STATE->rangeMap[y][propertyX]) {
          site->propertyMoves = MAP_STATE->rangeMap[y][propertyX];
        }
      }
    }
  }
}

/* End-actions step 7. For each record that is not skipped, fills the range map
 * with an Infantry's moves (FAR_AWAY) from the record's tile, then visits every
 * reachable tile that holds one of the current player's units. An Infantry or
 * Mech lowers unk_09 to the range map value there if that is smaller; a unit
 * with gUnitFitsLander set that waits for a T-Copter (transportWanted 2) does the
 * same for unk_0a. */
void func_02092134(void) {
  AiSite* record = data_0229e5bc;
  MapUnit* unit;
  int slot;
  int i;
  int x;
  int y;

  for (i = 0; i < data_021619c0->recordCount; i++, record++) {
    if (record->kind == 0xfd) {
      continue;
    }
    RangeMap_FillMoves(record->x, record->y, UNIT_INFANTRY, FAR_AWAY, 0);
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
        if (slot == 0) {
          continue;
        }
        if (Unit_IsCurrentPlayers(slot) != 1) {
          continue;
        }
        unit = &MAP_STATE->units[slot];
        if (unit->type == UNIT_INFANTRY || unit->type == UNIT_MECH) {
          if (record->unk_09 > MAP_STATE->rangeMap[y][x]) {
            record->unk_09 = MAP_STATE->rangeMap[y][x];
          }
        }
        if (gUnitFitsLander[unit->type] == 0 || unit->transportWanted != 2) {
          continue;
        }
        if (record->unk_0a > MAP_STATE->rangeMap[y][x]) {
          record->unk_0a = MAP_STATE->rangeMap[y][x];
        }
      }
    }
  }
}

/* Writes one AiSite record for each property that Map_IsFreeDeployCity accepts
 * and ends the list with x 0xff. Returns the number of records. A city counts
 * as a base. */
int AiSites_Collect(void) {
  int count = 0;
  Unk_02080440_6088* property;
  AiSite* site = data_0229e5bc;

  for (property = MAP_STATE->unk_6088; property->terrain != MAP_SITE_END;
       property++) {
    if (Map_IsFreeDeployCity(property->x, property->y) != 1) {
      continue;
    }
    site->x = property->x;
    site->y = property->y;
    site->type = gTerrainFactoryKind[TILE_TERRAIN(property->x, property->y)];
    if (TILE_TERRAIN(property->x, property->y) == TERRAIN_CITY) {
      site->type = SITE_BASE;
    }
    site->kind = 127;
    site->group = -1;
    site->unitBalance = 0;
    site->enemyCount = 0;
    site->enemyDistance = 0xff;
    site->propertyMoves = 0xff;
    site->unk_09 = 0xff;
    site->unk_0a = 0xff;
    site->score = 0;
    site++;
    count++;
  }
  site->x = 0xff;
  return count;
}

/* End-actions step 1. Like AiTurn_TryBuildFighter, for unit type 14
 * (UNIT_ANTI_AIR), but gated on the sea bit of domainsOwn, which does not fit
 * that type, and it also clears needAntiAir in the site group of
 * AiBuildSlot_PickSafeSite's pick. Both are unexplained, so the name stays.
 * Returns 1 when it queued an action. */
BOOL func_02091eb0(void) {
  int index;
  AiSiteGroup* record;

  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return 0;
  }
  if (gAiProfile->units[UNIT_ANTI_AIR - 1].unk_00 == 0) {
    return 0;
  }
  if ((u32)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_ANTI_AIR) * 10) >
      MAP_STATE->players[MAP_STATE->currentPlayer].funds) {
    return 0;
  }
  index = AiBuildSlot_PickSafeSite();
  if (index == -1) {
    return 0;
  }
  record = &gAiSiteGroups[data_0229e5c0[index][0]];
  record->needAntiAir = 0;
  gUnitAction.target = UNIT_ANTI_AIR;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* End-actions step 2. AiSite_PickBestBalance picks a site record (-1: none), then
 * AiSite_SumNearbyUnitHp fills the range map from it. If AiSite_ChooseUnitToBuild finds nothing
 * for the site, its kind becomes 0xfe, else AiTurn_QueueBuildAtSite queues an action
 * for it. Returns 1 when it picked a site. */
BOOL AiTurn_TrySiteAction(void) {
  int site = AiSite_PickBestBalance();

  if (site == -1) {
    return 0;
  }
  AiSite_SumNearbyUnitHp(site);
  if (AiSite_ChooseUnitToBuild(site) != -1) {
    AiTurn_QueueBuildAtSite(site);
    return 1;
  }
  data_0229e5bf[site][0] = 0xfe;
  return 1;
}
