#include "types.h"
#include "game/map_unit.h"
#include "game/math.h"
#include "game/Unk_02080440.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/unit_action.h"
#include "game/ai_profile.h"
#include "game/unit_stats.h"
#include "game/ai_unit_action.h"
#include "game/event_script.h"
#include "game/script_slot.h"
#include "game/ai/ai_build_buy.h"
#include "game/ai/ai_build_pick.h"
#include "game/ai/ai_sites.h"
#include "game/ai/ai_power.h"
#include "game/ai/ai_enemy_within.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

#define SHOWN_HP(unit) ((unit)->hp != 0 ? HwDiv32((unit)->hp - 1, 10) + 1 : 0)
#define CURRENT_PLAYER (MAP_STATE->players[MAP_STATE->currentPlayer])
#define ORDER_COUNT 7
#define HQ_SAFE_RADIUS 6
/* Action number sent for the build; AiShowAction_Step2 runs
 * AiShowAction_BuildUnitAlt for it. */
#define ACTION_BUILD_AT_SITE 14

extern ScriptCommand gMapScrollScript[];
extern u32 gLinkPlay;

int Player_GetRepairHp(int playerIndex, int slot, int powerState);
int UnitType_GetCostDiv10(int unitType);
int Player_GetCoCostPercent(int player, int coSlot, int powerState,
                            int unitType);
int Unit_CanRepairAt(MapUnit* unit, int x, int y);
int Rng_Next(void);
void func_020f5878(int x, int y);
s16 ScriptSlot_Find(const ScriptCommand* script);
MapUnit* CurrentPlayer_BuyUnit(int x, int y, int unitType);
int GetUnitMaxRange(int player, int unitType);
void UnitAction_Send(int action, int unitIndex, u8 target, u8 arg);
void func_020b81c0(MapUnit* unit);

/* End-actions build at the AiSite build->buildSite, in two calls: the first
 * scrolls the map to the site, the next ones wait for the scroll script to end,
 * then buy gUnitAction.target there with aiOrder gUnitAction.unk_07 and no
 * targets. unk_0c_4 of the new unit: Infantry and Mech fill the profile's
 * quotas of units with it set (unk_08[1]) and clear (unk_08[0]) first; then
 * units with no weapon set it, the rest clear it when the roll is below the
 * unit type's unk_05. Returns FALSE when no build is in progress. */
BOOL AiBuild_BuyAtSite(void) {
  MapUnit* unit;
  AiBuildState* build = data_021619c0;
  AiSite* site = &data_0229e5bc[build->buildSite];

  switch (build->buildStep) {
    case 1:
      build->buildStep++;
      func_020f5878(site->x, site->y);
      break;
    case 2:
      if (ScriptSlot_Find(gMapScrollScript) != -1) {
        break;
      }
      data_021619c0->buildStep = 0;
      unit = CurrentPlayer_BuyUnit(site->x, site->y, gUnitAction.target);
      unit->aiOrder = gUnitAction.unk_07;
      unit->serviceWanted = 0;
      unit->transportWanted = 0;
      unit->aiTargetX = MAP_UNIT_NO_TARGET;
      unit->aiTargetY = MAP_UNIT_NO_TARGET;
      if (unit->type == UNIT_INFANTRY || unit->type == UNIT_MECH) {
        if (Ai_CountFootUnitsByUnk0c4(unit - MAP_STATE->units, 1) <
            gAiProfile->unk_08[1]) {
          unit->unk_0c_4 = 1;
          goto done;
        }
        if (Ai_CountFootUnitsByUnk0c4(unit - MAP_STATE->units, 0) <
            gAiProfile->unk_08[0]) {
          unit->unk_0c_4 = 0;
          goto done;
        }
      }
      if (GetUnitMaxRange(MAP_STATE->currentPlayer, unit->type) == 0) {
        unit->unk_0c_4 = 1;
      } else if (gAiTurnState->roll <
                 gAiProfile->units[unit->type - 1].unk_05) {
        unit->unk_0c_4 = 0;
      } else {
        unit->unk_0c_4 = 1;
      }
    done:
      MAP_STATE->actingUnitDestX = site->x;
      MAP_STATE->actingUnitDestY = site->y;
      if (gLinkPlay != 0) {
        UnitAction_Send(ACTION_BUILD_AT_SITE, gUnitAction.target,
                        gUnitAction.unk_07, 0);
      }
      func_020b81c0(unit);
      break;
    default:
      return FALSE;
  }
  return TRUE;
}

/* Counts the current player's Infantry and Mech (slots 1-50), except the one
 * in skipSlot, whose unk_0c_4 bit equals flag. */
int Ai_CountFootUnitsByUnk0c4(int skipSlot, int flag) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type != UNIT_NONE && slot != skipSlot &&
        unit->type <= UNIT_MECH) {
      if (unit->unk_0c_4 == flag) {
        count++;
      }
    }
  }
  return count;
}

/* Picks the MapUnit.aiOrder (0-6) of a unit type about to be built, at
 * random with the weights in gAiProfile->units[unitType - 1].orderWeights. Bits of
 * `excluded` give orders weight 0. Order 6 is also left out when the current
 * player has a base near the HQ (Ai_HasOwnBaseNearHq) and no enemy within
 * HQ_SAFE_RADIUS of the HQ. Returns 0 for sea units and for types whose first
 * weight is negative. The caller passes the result through gUnitAction.unk_07
 * to AiBuild_BuyAtSite. */
u8 AiBuild_RollOrder(int unitType, u8 excluded) {
  u8 weights[ORDER_COUNT];
  u32 total;
  u8 index = unitType - 1;
  int i;
  u8 roll;

  if (gAiProfile->units[index].orderWeights[0] < 0) {
    return 0;
  }
  if (Ov000_gUnitStats[index + 1].moveGroup & MOVE_GROUP_SEA) {
    return 0;
  }
  if (Ai_HasOwnBaseNearHq() == TRUE) {
    if (!Ai_HasEnemyWithin(CURRENT_PLAYER.hqX & 0x7f, CURRENT_PLAYER.hqY & 0x7f,
                           HQ_SAFE_RADIUS)) {
      excluded |= 1 << 6;
    }
  }
  total = 0;
  for (i = 0; i < ORDER_COUNT; i++) {
    if (excluded & (1 << i)) {
      weights[i] = 0;
    } else {
      weights[i] = gAiProfile->units[index].orderWeights[i];
      total += weights[i];
    }
  }
  roll = (u32)Rng_Next() % total;
  for (i = 0; i < ORDER_COUNT; i++) {
    if (weights[i] > roll) {
      return i;
    }
    roll -= weights[i];
  }
  return 0;
}

/* Common tail of the AiTurn_TryBuild* functions once gUnitAction.target holds
 * the unit type. Passes the type to AiBuild_RollOrder (guess: it picks a value the
 * action carries in unk_07), marks the site as used and remembers its index. */
void AiTurn_QueueBuildAtSite(int index) {
  gUnitAction.unk_07 = AiBuild_RollOrder(gUnitAction.target, 0);
  data_0229e5bc[index].kind = SITE_KIND_SKIPPED;
  data_021619c0->unk_07++;
  data_021619c0->buildSite = index;
  data_021619c0->buildStep = 1;
}

/* Step 0 of the end-of-turn phase: fills the AiBuildState counts, counts the
 * current player's units, and totals AiUnit_GetRepairValue over those that can be
 * repaired where they stand (times 1.5, at least 1000). Meaning of the total
 * unknown. */
void AiEndActions_Start(void) {
  int i;
  MapUnit* unit;
  BOOL flag;

  data_021619c0->recordCount = AiSites_Collect();
  data_021619c0->slotCount = AiSites_BuildGroups();
  flag = AiPlayer_FrontCoNearerToSuper() == 1;
  data_021619c0->unk_07 = 0;
  data_021619c0->unk_0c = 0;
  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != 0) {
      data_021619c0->unk_07++;
      if (Unit_CanRepairAt(unit, unit->x, unit->y)) {
        data_021619c0->unk_0c +=
            AiUnit_GetRepairValue(MAP_STATE->currentPlayer, flag, unit);
      }
    }
  }
  data_021619c0->unk_0c = data_021619c0->unk_0c * 3 >> 1;
  if (data_021619c0->unk_0c < 1000) {
    data_021619c0->unk_0c = 1000;
  }
  data_021619c0->savingTarget = 0;
  data_021619c0->savingSkips = 2;
}

/* Like Unit_GetValuePerHp, but for a given CO slot and with that CO's power
 * state taken as 0 (no power active). */
int Unit_GetValuePerHpNoPower(int player, int coSlot, int unitType) {
  int cost = UnitType_GetCostDiv10(unitType);

  return HwDiv32(
      cost * (Player_GetCoCostPercent(player, coSlot, 0, unitType) + 100), 100);
}

/* Value in funds / 10 of the HP one repair would give the unit: the player's
 * repair amount, limited to the shown HP the unit is missing, times the value
 * per HP. 0 for a unit above 90 hp. */
int AiUnit_GetRepairValue(int player, int coSlot, MapUnit* unit) {
  int repair;
  int missing;

  if (unit->hp >= 91) {
    return 0;
  }
  repair = Player_GetRepairHp(player, coSlot, 0);
  missing = 10 - SHOWN_HP(unit);
  if (repair > missing) {
    repair = missing;
  }
  return repair * Unit_GetValuePerHpNoPower(player, coSlot, unit->type);
}
