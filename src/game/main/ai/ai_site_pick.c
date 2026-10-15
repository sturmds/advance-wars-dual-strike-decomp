#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/ai_site_group.h"
#include "game/ai/ai_end_actions.h"
#include "game/ai/ai_site_pick.h"
#include "game/ai/ai_build_checks.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define SCRATCH_FAR 126
#define BUILD_RATING_MAX 40
#define ANTI_SUB_RATING_MAX 60
#define AIRPORT_THREAT_LIMIT 40
#define THREAT_LIMIT 50
#define SLOT_USED_UP 0xff
#define SAFE_THREAT_LIMIT 50
#define GROUP_USED_UP 0xff
#define GROUP_PENALTY 255

/* Returns the index of the first site of a given type (SITE_TYPE_ANY: every
 * type) that is not skipped and that AiSite_NearUnguardedOozium does not reject, or -1. */
int AiSite_PickFirst(int siteType) {
  AiSite* site = data_0229e5bc;
  int i;

  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    return i;
  }
  return -1;
}

/* AiSite_PickMostEnemies without the group test. */
int AiSite_PickMostEnemiesAnyGroup(int siteType) {
  AiSite* site;
  int i;
  int bestCount;
  int best;

  bestCount = 0;
  i = 0;
  site = data_0229e5bc;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (bestCount < site->enemyCount) {
      bestCount = site->enemyCount;
      best = i;
    }
  }
  return best;
}

/* AiSite_PickNearestToProperty without the group test. */
int AiSite_PickNearestToPropertyAnyGroup(int siteType) {
  AiSite* site;
  int i;
  int bestMoves;
  int best;

  site = data_0229e5bc;
  bestMoves = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (bestMoves > site->propertyMoves) {
      bestMoves = site->propertyMoves;
      best = i;
    }
  }
  return best;
}

/* Picks the site of a given type (SITE_TYPE_ANY: every type) with the lowest
 * unk_09, the range-map value of the nearest Infantry or Mech. Returns the
 * site index, or -1. */
int AiSite_PickNearestFoot(int siteType) {
  AiSite* site;
  int i;
  int bestValue;
  int best;

  site = data_0229e5bc;
  bestValue = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (bestValue > site->unk_09) {
      bestValue = site->unk_09;
      best = i;
    }
  }
  return best;
}

/* AiSite_PickNearestFoot, limited to sites whose group has a property in
 * reach that the current team does not own (AiSiteGroup.propertyCount). */
int AiSite_PickNearestFootWithProperty(int siteType) {
  AiSite* site;
  int i;
  int bestValue;
  int best;

  site = data_0229e5bc;
  bestValue = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (gAiSiteGroups[site->group].propertyCount == 0) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (bestValue > site->unk_09) {
      bestValue = site->unk_09;
      best = i;
    }
  }
  return best;
}

/* Picks a site of a given type for a Lander. unk_09 is the range-map value of
 * the nearest foot unit, unk_0a that of a unit waiting for a T-Copter. A site
 * is passed over when unk_09 is above the best unk_0a so far, or when its unk_0a
 * ties the best and the best site had unk_0a at least its unk_09. Returns the
 * site index, or -1. */
int AiSite_PickForLander(int siteType) {
  AiSite* site;
  int i;
  int bestValue;
  int best;
  int bestWasFarther;

  bestWasFarther = 0;
  i = 0;
  site = data_0229e5bc;
  bestValue = 0x7fff;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (bestValue < site->unk_09) {
      continue;
    }
    if (bestValue == site->unk_0a && bestWasFarther == 1) {
      continue;
    }
    bestValue = site->unk_0a;
    best = i;
    if (site->unk_0a >= site->unk_09) {
      bestWasFarther = 1;
    } else {
      bestWasFarther = 0;
    }
  }
  return best;
}

/* Like AiSite_PickNearestEnemy without the group test, but a base site whose
 * group has unk_03_2 set counts GROUP_PENALTY further away. */
int AiSite_PickNearestEnemyPenalized(int siteType) {
  AiSite* site;
  int i;
  int bestDistance;
  int best;
  int distance;

  site = data_0229e5bc;
  bestDistance = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    distance = site->enemyDistance;
    if (site->type == SITE_BASE && gAiSiteGroups[site->group].unk_03_2) {
      distance += GROUP_PENALTY;
    }
    if (bestDistance > distance) {
      bestDistance = distance;
      best = i;
    }
  }
  return best;
}

/* Like AiSite_PickNearestToProperty, but picks the site whose nearest
 * enemy unit is closest (enemyDistance). Returns the site index, or -1. */
int AiSite_PickNearestEnemy(int siteType, int group) {
  AiSite* site;
  int i;
  int bestDistance;
  int best;

  site = data_0229e5bc;
  bestDistance = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (site->group != group) {
      continue;
    }
    if (bestDistance > site->enemyDistance) {
      bestDistance = site->enemyDistance;
      best = i;
    }
  }
  return best;
}

/* Like AiSite_PickNearestToProperty, but picks the site with the most
 * enemy units near it (enemyCount). Returns the site index, or -1. */
int AiSite_PickMostEnemies(int siteType, int group) {
  AiSite* site;
  int i;
  int bestCount;
  int best;

  bestCount = 0;
  i = 0;
  site = data_0229e5bc;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (site->group != group) {
      continue;
    }
    if (bestCount < site->enemyCount) {
      bestCount = site->enemyCount;
      best = i;
    }
  }
  return best;
}

/* Picks the site with the highest unitBalance (enemy units near it minus own
 * team's), skipping skipped sites and those AiSite_NearUnguardedOozium rejects. The balance
 * must beat 0. Returns the site index, or -1. */
int AiSite_PickBestBalance(void) {
  AiSite* site;
  int i;
  int bestBalance;
  int best;

  bestBalance = 0;
  i = 0;
  site = data_0229e5bc;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (bestBalance < site->unitBalance) {
      bestBalance = site->unitBalance;
      best = i;
    }
  }
  return best;
}

/* Takes the group with the highest threat that is not used up and asks
 * AiSite_PickMostEnemies for a site in it. Returns that site when there is
 * one; otherwise marks a group used up and searches again. The group pointer
 * and the best threat are not reset between searches, so the second search
 * starts where the first stopped, and the mark lands on the entry after the
 * last group. Returns -1 when no group is left. */
int AiSite_PickInMostThreatenedGroup(int siteType) {
  AiSiteGroup* group = gAiSiteGroups;
  int bestThreat = 0;
  int i;
  int found;
  int site;

  func_020937dc();
  for (;;) {
    found = -1;
    for (i = 0; i < data_021619c0->slotCount; i++, group++) {
      if (group->unk_01 != GROUP_USED_UP && bestThreat < group->threat) {
        bestThreat = group->threat;
        found = i;
      }
    }
    if (found == -1) {
      return -1;
    }
    site = AiSite_PickMostEnemies(siteType, found);
    if (site != -1) {
      return site;
    }
    group->unk_01 = GROUP_USED_UP;
  }
}

/* Among the sites that are not skipped and not rejected by AiSite_NearUnguardedOozium,
 * with the given site type (SITE_TYPE_ANY: every type) and group, picks the
 * one with the lowest propertyMoves. Returns the site index, or -1. */
int AiSite_PickNearestToProperty(int siteType, int group) {
  AiSite* site;
  int i;
  int bestMoves;
  int best;

  site = data_0229e5bc;
  bestMoves = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (siteType != SITE_TYPE_ANY && site->type != siteType) {
      continue;
    }
    if (site->group != group) {
      continue;
    }
    if (bestMoves > site->propertyMoves) {
      bestMoves = site->propertyMoves;
      best = i;
    }
  }
  return best;
}

/* Takes the first group in the list that is not used up and has skipFoot
 * clear, and asks AiSite_PickNearestToProperty for a site in it. Returns that
 * site when there is one; otherwise marks a group used up and searches again.
 * The group pointer is not reset between searches, so the second search
 * starts where the first stopped (as in AiSite_PickInMostThreatenedGroup).
 * Returns -1 when no group is left. */
int AiSite_PickForFootUnit(void) {
  AiSiteGroup* group = gAiSiteGroups;
  int i;
  int found;
  int site;

  func_020937dc();
  for (;;) {
    found = -1;
    for (i = 0; i < data_021619c0->slotCount; i++, group++) {
      if (group->unk_01 != GROUP_USED_UP && !group->skipFoot) {
        found = i;
        break;
      }
    }
    if (found == -1) {
      return -1;
    }
    site = AiSite_PickNearestToProperty(SITE_BASE, found);
    if (site != -1) {
      return site;
    }
    group->unk_01 = GROUP_USED_UP;
  }
}

/* Takes the first site group with needAntiAir set that is not used up, and asks
 * AiSite_PickMostEnemies and then AiSite_PickNearestEnemy for a site for it. Returns the site
 * when both enemy threat values of its tile are below SAFE_THREAT_LIMIT;
 * otherwise marks the slot used up and searches again. The slot pointer is
 * not reset between searches, so a new search starts at the slot just marked
 * while i starts at 0 again. Returns -1 when no slot is left. */
int AiBuildSlot_PickSafeSite(void) {
  AiSiteGroup* slot = gAiSiteGroups;
  int i;
  int found;
  int site;
  int tile;

  func_020937dc();
  for (;;) {
    found = -1;
    for (i = 0; i < data_021619c0->slotCount; i++, slot++) {
      if (slot->unk_01 != SLOT_USED_UP && slot->needAntiAir) {
        found = i;
        break;
      }
    }
    if (found == -1) {
      return -1;
    }
    site = AiSite_PickMostEnemies(1, found);
    if (site != -1) {
      site = AiSite_PickNearestEnemy(1, found);
    }
    if (site != -1) {
      tile = MAP_TILE_INDEX(data_0229e5bc[site].x, data_0229e5bc[site].y);
      if (MAP_STATE->mapTiles->threatMax[tile] < SAFE_THREAT_LIMIT &&
          MAP_STATE->mapTiles->threatSum[tile] < SAFE_THREAT_LIMIT) {
        return site;
      }
    }
    slot->unk_01 = SLOT_USED_UP;
  }
}

/* Picks the site to build at, trying each rule until one finds a site: the
 * most threatened group (only when the map has two or more groups), most
 * enemies in any group, nearest enemy, then the first site. Returns the site
 * index, or -1. */
int AiSite_PickForBuild(int siteType) {
  int site = -1;

  if (data_021619c0->slotCount >= 2) {
    site = AiSite_PickInMostThreatenedGroup(siteType);
  }
  if (site == -1) {
    site = AiSite_PickMostEnemiesAnyGroup(siteType);
  }
  if (site == -1) {
    site = AiSite_PickNearestEnemyPenalized(siteType);
  }
  if (site == -1) {
    site = AiSite_PickFirst(siteType);
  }
  return site;
}

/* Among the airport sites whose tile has enemy threatMax plus threatSum of
 * at most THREAT_LIMIT, picks the one with the most enemyCount (above 0).
 * Returns the site index, or -1. */
int AiSite_PickBlackBombAirportMostEnemies(void) {
  AiSite* site;
  int i;
  int bestEnemies;
  int best;
  int tile;

  site = data_0229e5bc;
  bestEnemies = 0;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    tile = MAP_TILE_INDEX(site->x, site->y);
    if (MAP_STATE->mapTiles->threatMax[tile] +
            MAP_STATE->mapTiles->threatSum[tile] >
        THREAT_LIMIT) {
      continue;
    }
    if (site->type != SITE_AIRPORT) {
      continue;
    }
    if (bestEnemies < site->enemyCount) {
      bestEnemies = site->enemyCount;
      best = i;
    }
  }
  return best;
}

/* Among the airport sites whose tile has enemy threatMax plus threatSum of
 * at most THREAT_LIMIT, picks the one with the smallest enemyDistance.
 * Returns the site index, or -1. */
int AiSite_PickBlackBombAirportNearestEnemy(void) {
  AiSite* site;
  int i;
  int bestDistance;
  int best;
  int tile;

  site = data_0229e5bc;
  bestDistance = 0x7fff;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    tile = MAP_TILE_INDEX(site->x, site->y);
    if (MAP_STATE->mapTiles->threatMax[tile] +
            MAP_STATE->mapTiles->threatSum[tile] >
        THREAT_LIMIT) {
      continue;
    }
    if (site->type != SITE_AIRPORT) {
      continue;
    }
    if (bestDistance > site->enemyDistance) {
      bestDistance = site->enemyDistance;
      best = i;
    }
  }
  return best;
}

/* Picks the airport site to build a Black Bomb at: the one with the most
 * enemies near it, or when none qualifies, the one nearest an enemy.
 * Returns the site index, or -1. */
int AiSite_PickBlackBombAirport(void) {
  int index;

  index = AiSite_PickBlackBombAirportMostEnemies();
  if (index != -1) {
    return index;
  }
  return AiSite_PickBlackBombAirportNearestEnemy();
}

/* Among the sites of type 1 that are not skipped and not rejected by
 * AiSite_NearUnguardedOozium, picks the one with the highest score (AiSite_ScorePiperunnerNeed).
 * Returns the site index, or -1 when no score is above 0. */
int AiSite_PickPiperunnerSite(void) {
  AiSite* site;
  int i;
  int bestScore;
  int best;

  site = data_0229e5bc;
  bestScore = 0;
  best = -1;
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (site->type != SITE_BASE) {
      continue;
    }
    if (bestScore < site->score) {
      bestScore = site->score;
      best = i;
    }
  }
  return best;
}

/* Picks the airport site to build a Fighter at: not skipped, not one where
 * AiSite_NearUnguardedOozium returns 1, and with both enemy threat values of its tile below
 * AIRPORT_THREAT_LIMIT. Prefers the most enemies near the site, then the nearest enemy.
 * Returns the site index, or -1. */
int AiSite_PickAirport(void) {
  /* needed to match: declaration and initialisation order */
  AiSite* site;
  int i;
  int bestEnemyCount;
  int bestEnemyDistance;
  int best;
  int tile;

  bestEnemyCount = 0;
  i = 0;
  site = data_0229e5bc;
  bestEnemyDistance = 0x7fff;
  best = -1;
  for (; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (site->type != SITE_AIRPORT) {
      continue;
    }
    tile = MAP_TILE_INDEX(site->x, site->y);
    if (MAP_STATE->mapTiles->threatMax[tile] >= AIRPORT_THREAT_LIMIT ||
        MAP_STATE->mapTiles->threatSum[tile] >= AIRPORT_THREAT_LIMIT) {
      continue;
    }
    if (bestEnemyCount > site->enemyCount) {
      continue;
    }
    if (bestEnemyCount == site->enemyCount &&
        bestEnemyDistance < site->enemyDistance) {
      continue;
    }
    bestEnemyDistance = site->enemyDistance;
    bestEnemyCount = site->enemyCount;
    best = i;
  }
  return best;
}

/* Like AiSite_PickPortForCruiser, but after Ai_CountEnemySubReach: picks the
 * port whose tile enemy Subs reach the most (more than 0), among ports rated
 * below 60 for a Cruiser. The first of equal ports wins. Returns the site
 * index, or -1. */
int AiSite_PickPortAgainstSubs(void) {
  AiSite* site;
  int i;
  int value;
  int bestValue;
  int best;

  site = data_0229e5bc;
  bestValue = 0;
  best = -1;
  Ai_CountEnemySubReach();
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (site->type != SITE_PORT) {
      continue;
    }
    value = MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(site->x, site->y)];
    if (bestValue >= value) {
      continue;
    }
    if (AiBuild_RateSiteDanger(site->x, site->y, UNIT_CRUISER) <
        ANTI_SUB_RATING_MAX) {
      bestValue = value;
      best = i;
    }
  }
  return best;
}

/* Picks the port site (type 3) to build a Cruiser at: not skipped, not
 * rejected by AiSite_NearUnguardedOozium, and rated below BUILD_RATING_MAX for a Cruiser
 * by AiBuild_RateSiteDanger. Of those, the one with the lowest value in
 * MapTiles.rangeScratch (filled by Ai_BuildEnemySubReachMap), at most SCRATCH_FAR; a
 * later site wins a tie. Returns the site index, or -1. */
int AiSite_PickPortForCruiser(void) {
  AiSite* site;
  int i;
  int value;
  int bestValue;
  int best;

  site = data_0229e5bc;
  bestValue = SCRATCH_FAR;
  best = -1;
  Ai_BuildEnemySubReachMap();
  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (site->kind >= SITE_KIND_SKIPPED) {
      continue;
    }
    if (AiSite_NearUnguardedOozium(site->x, site->y) == 1) {
      continue;
    }
    if (site->type != SITE_PORT) {
      continue;
    }
    value = MAP_STATE->mapTiles->rangeScratch[MAP_TILE_INDEX(site->x, site->y)];
    if (bestValue < value) {
      continue;
    }
    if (AiBuild_RateSiteDanger(site->x, site->y, UNIT_CRUISER) <
        BUILD_RATING_MAX) {
      bestValue = value;
      best = i;
    }
  }
  return best;
}

/* True when some AiSite record has the given type (SITE_TYPE_ANY: when there
 * is any record). Skipped records count. */
BOOL AiSites_HasType(int siteType) {
  AiSite* site = data_0229e5bc;
  int i;

  for (i = 0; i < data_021619c0->recordCount; i++, site++) {
    if (siteType == SITE_TYPE_ANY || site->type == siteType) {
      return TRUE;
    }
  }
  return FALSE;
}
