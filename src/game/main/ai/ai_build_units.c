#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/math.h"
#include "game/ai_unit_action.h"
#include "game/ai_site.h"
#include "game/map_unit.h"
#include "game/ai_site_group.h"
#include "game/ai/ai_build_units.h"
#include "game/ai/ai_build_buy.h"
#include "game/ai/ai_end_actions.h"
#include "game/ai/ai_threat_maps.h"
#include "game/ai/ai_count_units.h"
#include "game/ai/ai_site_pick.h"
#include "game/ai/ai_build_pick.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))
#define UNLIMITED_MOVE 110

extern s8 data_0229e5c0[][12];

int Unit_GetValuePerHp(int playerIndex, int unitType);
BOOL Map_IsCapturableByActingUnit(s16 x, s16 y);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int GetUnitMaxRange(int player, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Rng_Next(void);
void func_020b81bc(void);

/* End-actions step 3. Tries to start building a Fighter: needs an airport among
 * the current player's factories, the profile's flag for Fighters set, the
 * funds for one and a yes from Ai_HasHealthyFighter and Ai_EnemyHasAirUnit (not decompiled;
 * the first must not return 1, the second must be nonzero). AiSite_PickAirport
 * picks the factory (-1: none). The name is a guess from the unit type and the
 * checks. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildFighter(void) {
  int index;

  if (!(gAiTurnState->factoryDomainsOwn & AI_DOMAIN_AIR)) {
    return 0;
  }
  if (gAiProfile->units[UNIT_FIGHTER - 1].unk_00 == 0) {
    return 0;
  }
  if ((u32)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_FIGHTER) * 10) >
      MAP_STATE->players[MAP_STATE->currentPlayer].funds) {
    return 0;
  }
  if (Ai_HasHealthyFighter() == 1) {
    return 0;
  }
  if (Ai_EnemyHasAirUnit() == 0) {
    return 0;
  }
  Ai_BuildFighterThreatMaps();
  index = AiSite_PickAirport();
  if (index == -1) {
    return 0;
  }
  gUnitAction.target = UNIT_FIGHTER;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* End-actions step 4. Same checks and action as AiTurn_TryBuildCruiser (step 5)
 * with a different factory pick, AiSite_PickPortAgainstSubs. How the two picks differ is not
 * known, so the name stays. Returns 1 when it queued an action. */
BOOL func_02091c54(void) {
  int index;

  if (!(gAiTurnState->factoryDomainsOwn & AI_DOMAIN_SEA)) {
    return 0;
  }
  if (gAiProfile->units[UNIT_CRUISER - 1].unk_00 == 0) {
    return 0;
  }
  if ((u32)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_CRUISER) * 10) >
      MAP_STATE->players[MAP_STATE->currentPlayer].funds) {
    return 0;
  }
  index = AiSite_PickPortAgainstSubs();
  if (index == -1) {
    return 0;
  }
  gUnitAction.target = UNIT_CRUISER;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* End-actions step 5. Tries to start building a Cruiser: needs a port among
 * the current player's factories, the profile's flag for Cruisers set and the
 * funds for one. AiSite_PickPortForCruiser picks the factory (-1: none). The name is a
 * guess from the unit type and the checks. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildCruiser(void) {
  int index;

  if (!(gAiTurnState->factoryDomainsOwn & AI_DOMAIN_SEA)) {
    return 0;
  }
  if (gAiProfile->units[UNIT_CRUISER - 1].unk_00 == 0) {
    return 0;
  }
  if ((u32)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_CRUISER) * 10) >
      MAP_STATE->players[MAP_STATE->currentPlayer].funds) {
    return 0;
  }
  index = AiSite_PickPortForCruiser();
  if (index == -1) {
    return 0;
  }
  gUnitAction.target = UNIT_CRUISER;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* Chance in percent (0 to 80) that a foot unit is built as a Mech instead of
 * Infantry: none up to day 3, then profile byte unk_08[2] per day. */
u32 AiTurn_GetMechBuildPercent(void) {
  int chance = 0;

  if (MAP_STATE->day > 3) {
    chance = gAiProfile->unk_08[2] * MAP_STATE->day;
    if (chance > 80) {
      chance = 80;
    }
  }
  return chance;
}

/* End-actions step 6. Tries to start building a foot unit if funds cover an
 * Infantry. Takes the first factory that AiSite_PickForFootUnit,
 * AiSite_PickNearestToPropertyAnyGroup(1) or AiSite_PickFirst(1) return
 * (-1: none). A factory whose site group already has skipFoot set is skipped
 * when unk_08 reaches the profile's unk_08[0] + unk_08[1]. The unit is a Mech
 * when a roll of 0 to 99 is under AiTurn_GetMechBuildPercent() and funds cover 20 times its
 * value per HP, else an Infantry. The name and the meaning of the flags are
 * guesses. */
BOOL AiTurn_TryBuildFootUnit(void) {
  int index;
  u32 roll;
  AiSiteGroup* record;

  if ((u32)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_INFANTRY) * 10) >
      MAP_STATE->players[MAP_STATE->currentPlayer].funds) {
    return 0;
  }
  index = AiSite_PickForFootUnit();
  if (index == -1) {
    index = AiSite_PickNearestToPropertyAnyGroup(1);
  }
  if (index == -1) {
    index = AiSite_PickFirst(1);
  }
  if (index == -1) {
    return 0;
  }
  record = &gAiSiteGroups[data_0229e5c0[index][0]];
  if (record->skipFoot == 0) {
    record->skipFoot = 1;
  } else {
    data_021619c0->unk_08 = Ai_CountUnitsOfAiClass(1);
    if (data_021619c0->unk_08 >=
        gAiProfile->unk_08[0] + gAiProfile->unk_08[1]) {
      return 0;
    }
  }
  roll = HwMod32(Rng_Next(), 100);
  if (roll < AiTurn_GetMechBuildPercent() &&
      (u32)MAP_STATE->players[MAP_STATE->currentPlayer].funds >=
          (u32)(Unit_GetValuePerHp(MAP_STATE->currentPlayer, UNIT_MECH) * 20)) {
    gUnitAction.target = UNIT_MECH;
  } else {
    gUnitAction.target = UNIT_INFANTRY;
  }
  func_020b81bc();
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* End-actions step 9. AiTurn_PickUnitToBuild returns -1 when it finds nothing, else an
 * index that AiTurn_QueueBuildAtSite takes (the other steps pass it the same way). What
 * either does is not known yet. */
BOOL func_02091904(void) {
  int index;

  AiBuild_SumUnitHp();
  index = AiTurn_PickUnitToBuild(1);
  if (index == -1) {
    return 0;
  }
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* Run first by AiTurn_TryBuildTCopter. Unless AiSites_HasType(3) returns 1,
 * takes each site that AiSite_PickNearestFoot(2) returns in turn (-1: none left) and
 * fills the range map with Mech moves from it over the whole map. A site
 * is skipped (kind 0xfe) when a unit in reach stands on a tile that
 * Map_IsCapturableByActingUnit accepts, when an enemy vehicle in reach has
 * a weapon, or when the player's foot units in reach number fewer than
 * three per empty T-Copter in reach, the new one included. The first site
 * that passes gets a T-Copter. AiSites_ClearAllMarks then resets the skipped kinds.
 * Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildTCopterForFoot(void) {
  int slot;
  int index;
  MapUnit* unit;
  u8 copters;
  u8 foot;
  int x;
  AiSite* site;
  int y;

  if (AiSites_HasType(3) == 1) {
    return 0;
  }
  for (;;) {
    index = AiSite_PickNearestFoot(2);
    if (index == -1) {
      AiSites_ClearAllMarks();
      return 0;
    }
    site = &data_0229e5bc[index];
    RangeMap_FillMoves(site->x, site->y, UNIT_MECH, UNLIMITED_MOVE, 0);
    foot = 0;
    copters = 0;
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        slot =
            MAP_STATE->mapTiles->unitAt[x + MAP_STATE->mapTiles->rowStart[y]];
        if (slot == 0) {
          continue;
        }
        if (Map_IsCapturableByActingUnit(x, y) == 1) {
          goto skip;
        }
        unit = &MAP_STATE->units[slot];
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[(slot >> 6) + 1].team) {
          if (unit->type > UNIT_MECH &&
              GetUnitMaxRange(MAP_STATE->currentPlayer, unit->type) != 0) {
            goto skip;
          }
        } else if (Unit_IsCurrentPlayers(slot) == 1) {
          if (unit->type == UNIT_T_COPTER && unit->cargo[0] == 0) {
            copters++;
          }
          if (unit->type <= UNIT_MECH) {
            foot++;
          }
        }
      }
    }
    if ((copters + 1) * 3 > foot) {
      goto skip;
    }
    gUnitAction.target = UNIT_T_COPTER;
    AiTurn_QueueBuildAtSite(index);
    AiSites_ClearAllMarks();
    return 1;
  skip:
    site->kind = 0xfe;
  }
}

/* End-actions step 8, first sub-step. Tries to start building a T-Copter:
 * needs the funds for one and, when gAiTurnState->propertyRegions is 1 and
 * the profile's unk_08[11] is nonzero, a state->unk_00 (likely the day) no
 * greater than it. AiTurn_TryBuildTCopterForFoot runs first and ends the step when it
 * returns 1. AiSite_PickNearestFoot(2), then AiSite_PickFirst(2), pick the factory
 * (-1: none). Builds when T-Copters make up less than unk_08[3] percent of
 * data_021619c0->unk_08. The name is a guess from the unit type. Returns 1
 * when it queued an action. */
BOOL AiTurn_TryBuildTCopter(void) {
  Unk_02080440_48* state;
  int index;
  int copters;

  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_T_COPTER) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  if (gAiTurnState->propertyRegions == 1 && gAiProfile->unk_08[11] != 0 &&
      state->day > gAiProfile->unk_08[11]) {
    return 0;
  }
  if (AiTurn_TryBuildTCopterForFoot() == 1) {
    return 1;
  }
  index = AiSite_PickNearestFoot(2);
  if (index == -1) {
    index = AiSite_PickFirst(2);
    if (index == -1) {
      return 0;
    }
  }
  copters = Ai_CountUnitsOfType(UNIT_T_COPTER);
  if (data_021619c0->unk_08 == 0) {
    return 0;
  }
  if (HwDiv32(copters * 100, data_021619c0->unk_08) < gAiProfile->unk_08[3]) {
    gUnitAction.target = UNIT_T_COPTER;
  } else {
    return 0;
  }
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* End-actions step 8, last sub-step. Tries to start building an APC: needs
 * the funds for one and, when the profile's unk_08[10] is nonzero, a
 * state->unk_00 (likely the day) no greater than it. AiSite_PickNearestFootWithProperty(1) picks
 * the factory (-1: none). Builds when APCs and T-Copters together make up
 * less than unk_08[4] percent of data_021619c0->unk_08 (Ai_CountUnitsOfAiClass(1)).
 * The name is a guess from the unit type. Returns 1 when it queued an
 * action. */
BOOL AiTurn_TryBuildApc(void) {
  Unk_02080440_48* state;
  int index;
  int transports;

  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_APC) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  if (gAiProfile->unk_08[10] != 0 && state->day > gAiProfile->unk_08[10]) {
    return 0;
  }
  index = AiSite_PickNearestFootWithProperty(1);
  if (index == -1) {
    return 0;
  }
  transports = Ai_CountUnitsOfType(UNIT_APC);
  transports += Ai_CountUnitsOfType(UNIT_T_COPTER);
  if (data_021619c0->unk_08 == 0) {
    return 0;
  }
  if (HwDiv32(transports * 100, data_021619c0->unk_08) <
      gAiProfile->unk_08[4]) {
    gUnitAction.target = UNIT_APC;
  } else {
    return 0;
  }
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
