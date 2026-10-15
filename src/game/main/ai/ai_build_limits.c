#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/range_map.h"
#include "game/map_unit.h"
#include "game/math.h"
#include "game/ai_site.h"
#include "game/ai/ai_site_pick.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/ai_build_state.h"
#include "game/ai_profile.h"
#include "game/ai/ai_build_checks.h"
#include "game/ai/ai_count_units.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */
#define UNLIMITED_MOVE 110

#define CRUISER_GUARD_HP 40

#define OWNER(unit) MAP_UNIT_OWNER(MAP_STATE, unit)

#define UNIT_OWNER(slot) (((slot) >> 6) + 1)

/* RangeMap_FillMoves movement; guess: high enough to act as no limit.
 * Tiles next to the reach get ADJACENT. */
#define ANY_DISTANCE 110

#define ADJACENT 111

/* Result bit: the landmass has a port, or a shoal, and the current player
 * has a Lander or the AI profile wants sea transport (for a shoal, only
 * when the player started with a sea factory). */
#define LANDMASS_SEA_TRANSPORT 0x100

#define PLAYER_AT(players, offset) ((Player*)((u32)(players) + (offset)))

int UnitType_GetAttackKind(u8 unitType);

int GetCoRangeBonus(int player, int coSlot, int powerState, int unitType);

/* TRUE when the profile's build flag byte has any bit of mask set (masks used: 1
 * Piperunners, 4 Black Boats, 8 Landers). */
BOOL AiProfile_HasBuildFlag(int mask) {
  return (gAiProfile->unk_08[9] & mask) != 0;
}

/* Bits 2 and 3 of the profile's byte 9; the sea transport code reads them
 * to decide whether to look for a transport. */
BOOL AiProfile_WantsSeaTransport(void) {
  return (gAiProfile->unk_08[9] & 0xc) != 0;
}

/* Artillery counterpart of AiBuild_HasEnoughGroundIndirects, with the same
 * count: TRUE while the current player has at least one indirect ground
 * vehicle per three direct ones. When flags is 0 and the front CO gives
 * Artillery extra range, one more direct is counted, so Artillery gets built
 * a little sooner. */
BOOL AiBuild_HasEnoughIndirectsForArtillery(int flags) {
  int direct = 0;
  int indirect = 0;
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (Ov000_gUnitStats[unit->type].moveGroup &
                                    (MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
      if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_DIRECT) {
        direct++;
      }
      if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_INDIRECT) {
        indirect++;
      }
    }
  }
  if (flags == 0 &&
      GetCoRangeBonus(MAP_STATE->currentPlayer, 0, 0, UNIT_ARTILLERY) > 0) {
    direct++;
  }
  return direct <= indirect * 3;
}

/* Counts the current player's direct and indirect ground vehicles (tires and
 * tread; slots 1-50 only). TRUE when there are enough indirects: at least as
 * many as directs when the front CO gives Artillery extra range and flags is
 * 0, otherwise at least half as many. The build step skips indirect types
 * while this is TRUE. flags comes from the caller; its meaning is unknown. */
BOOL AiBuild_HasEnoughGroundIndirects(int flags) {
  int direct = 0;
  int indirect = 0;
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51; i++) {
    unit = &MAP_STATE->units[i];
    if (unit->type != UNIT_NONE && (Ov000_gUnitStats[unit->type].moveGroup &
                                    (MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
      if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_DIRECT) {
        direct++;
      }
      if (UnitType_GetAttackKind(unit->type) == ATTACK_KIND_INDIRECT) {
        indirect++;
      }
    }
  }
  if (flags == 0 &&
      GetCoRangeBonus(MAP_STATE->currentPlayer, 0, 0, UNIT_ARTILLERY) > 0) {
    if (indirect == 0) {
      return FALSE;
    }
    if (direct > indirect) {
      return FALSE;
    }
  } else if (direct > indirect * 2) {
    return FALSE;
  }
  return TRUE;
}
