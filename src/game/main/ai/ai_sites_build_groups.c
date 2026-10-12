#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/ai_site_group.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int GetUnitMaxRange(int player, int unitType);
BOOL Terrain_IsProperty(u8 terrain);
int Unit_IsTileOwnedByTeam(int unitIndex, int tile);
BOOL Unit_IsHarmless(MapUnit* unit);
int AiSite_RateLandmass(int x, int y);
void AiSiteGroup_RateSurroundings(int group);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define FAR_AWAY 110

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
      if (STATE->rangeMap[other->y][other->x] >= 0) {
        other->group = count;
      }
    }
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
        if (STATE->rangeMap[y][x] < 0) {
          continue;
        }
        slot = STATE->mapTiles->unitAt[TILE(x, y)];
        if (slot != 0) {
          unit = &STATE->units[slot];
          if (STATE->players[STATE->currentPlayer].team ==
              STATE->players[(slot >> 6) + 1].team) {
            if (GetUnitMaxRange(STATE->currentPlayer, unit->type) != 0) {
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
        if (Terrain_IsProperty(STATE->mapTiles->tiles[TILE(x, y)]) == 1 &&
            !Unit_IsTileOwnedByTeam((s16)STATE->currentUnitBase,
                                    STATE->mapTiles->tiles[TILE(x, y)])) {
          if (site->propertyMoves > STATE->rangeMap[y][x]) {
            site->propertyMoves = STATE->rangeMap[y][x];
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
