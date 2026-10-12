#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/terrain.h"
#include "game/range_map.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Unit_IsCurrentPlayers(s16 unitSlot);
BOOL Terrain_IsPropertyNotSilo(
    u8 tile); /* guess: the tile is a property other than a silo */
int UnitType_GetAttackKind(int type);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* RangeMap_FillMoves movement; guess: high enough to act as no limit. */
#define ANY_DISTANCE 110

/* Run by AiUnit_SeekSeaTransport for a direct ground unit, when the army
 * started with a ground factory and now has sea units or a port. Fills
 * the range map as a Tank standing on the unit's tile would, then counts
 * what is on that landmass: properties other than silos, own units that
 * have claimedLandmass set, and how many of those are Infantry or Mech. Returns
 * FALSE at once when an own base is among the properties.
 *
 * Sets claimedLandmass on the unit when the landmass has few such units for its
 * properties. Infantry and Mech: no Infantry or Mech with up to 3
 * properties, at most 1 with more. Other units: no marked unit with up to
 * 3 properties, no marked non-foot unit with up to 6, at most 1 above.
 * Returns whether claimedLandmass is set. */
BOOL AiUnit_ClaimLandmass(void) {
  /* needed to match: declaration order sets the registers */
  int units;
  int slot;
  int properties;
  int footUnits;
  int x;
  int y;
  MapUnit* unit;

  if (!(gAiTurnState->factoryDomainsOwn & AI_DOMAIN_GROUND)) {
    return FALSE;
  }
  if (!(gAiTurnState->domainsOwn & AI_DOMAIN_SEA)) {
    return FALSE;
  }
  if (!(Ov000_gUnitStats[STATE->actingUnit->type].moveGroup &
        (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
    return FALSE;
  }
  if (UnitType_GetAttackKind(STATE->actingUnit->type) != ATTACK_KIND_DIRECT) {
    return FALSE;
  }
  if (STATE->actingUnit->claimedLandmass) {
    return FALSE;
  }
  RangeMap_FillMoves(STATE->actingUnit->x, STATE->actingUnit->y, UNIT_TANK,
                     ANY_DISTANCE, 0);
  properties = 0;
  footUnits = 0;
  units = 0;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (Terrain_IsPropertyNotSilo(STATE->mapTiles->tiles[TILE(x, y)])) {
        if (STATE->unk_04 == (STATE->mapTiles->tiles[TILE(x, y)] & 0xe0) &&
            MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]) ==
                TERRAIN_BASE) {
          return FALSE;
        }
        properties++;
      }
      slot = STATE->mapTiles->unitAt[TILE(x, y)];
      if (slot == 0) {
        continue;
      }
      if (!Unit_IsCurrentPlayers(slot)) {
        continue;
      }
      unit = &STATE->units[slot];
      if (!unit->claimedLandmass) {
        continue;
      }
      units++;
      if (unit->type <= UNIT_MECH) {
        footUnits++;
      }
    }
  }
  if (properties == 0) {
    return FALSE;
  }
  if (STATE->actingUnit->type <= UNIT_MECH) {
    if (properties <= 3) {
      if (footUnits == 0) {
        STATE->actingUnit->claimedLandmass = 1;
      }
    } else if (footUnits <= 1) {
      STATE->actingUnit->claimedLandmass = 1;
    }
  } else if (properties <= 3) {
    if (units == 0) {
      STATE->actingUnit->claimedLandmass = 1;
    }
  } else if (properties <= 6) {
    if (units - footUnits == 0) {
      STATE->actingUnit->claimedLandmass = 1;
    }
  } else if (units - footUnits <= 1) {
    STATE->actingUnit->claimedLandmass = 1;
  }
  return STATE->actingUnit->claimedLandmass ? TRUE : FALSE;
}
