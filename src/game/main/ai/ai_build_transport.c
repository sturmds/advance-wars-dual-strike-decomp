#include "types.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"
#include "game/ai_site.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/co_data.h"
#include "game/ai/ai_build_transport.h"
#include "game/ai/ai_build_buy.h"
#include "game/ai/ai_end_actions.h"
#include "game/ai/ai_site_pick.h"
#include "game/ai/ai_build_checks.h"
#include "game/ai/ai_count_units.h"

/* The functions are in reverse address order: mwccarm emits a file's
 * functions last to first. */

/* data_027c027c->unk_48[data_027c0284], written so that the compiler loads
 * the index first, as the original does. */
#define STATE                                                             \
  (*(Unk_02080440_48**)((u8*)(data_027c0284 * sizeof(Unk_02080440_48*)) + \
                            (u32)data_027c027c + (u32) &                  \
                        ((Unk_02080440*)0)->unk_48))
#define FAR_AWAY 110
/* Default movement cost chart of the first CO record. */
#define CHART \
  ((const s8(*)[32])gCoTable[0].blocks[0].movementCost[MOVE_CHART_DEFAULT])

int Unit_GetValuePerHp(int playerIndex, int unitType);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Player_GetUnitMoveRange(int player, int unitType);
int Map_IsCapturableByActingUnit(s16 x, s16 y);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
int UnitType_GetAttackKind(u8 unitType);

/* TRUE when a Lander's move type has a movement cost over 0 on the terrain
 * of the given map tile. */
BOOL Lander_CanEnterTile(int tile) {
  return CHART[Ov000_gUnitStats[UNIT_LANDER].moveType][MAP_TILE_TERRAIN(tile)] >
         0;
}

/* One step of a flood fill in rangeMap: every marked tile (not negative)
 * whose mark differs from `mark` and that a Lander can sail on
 * (Lander_CanEnterTile) gives `mark` to each unmarked neighbour. AiTurn_TryBuildLanderNearSite
 * passes marks from 111 up. */
void RangeMap_SpreadOverSea(u8 mark) {
  int x;
  int y;

  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (MAP_STATE->rangeMap[y][x] < 0 || mark == MAP_STATE->rangeMap[y][x]) {
        continue;
      }
      if (!Lander_CanEnterTile(
              MAP_STATE->mapTiles
                  ->tiles[x + MAP_STATE->mapTiles->rowStart[y]])) {
        continue;
      }
      if (x != 0 && MAP_STATE->rangeMap[y][x - 1] < 0) {
        MAP_STATE->rangeMap[y][x - 1] = mark;
      }
      if (x != MAP_STATE->unk_1c4->mapWidth - 1 &&
          MAP_STATE->rangeMap[y][x + 1] < 0) {
        MAP_STATE->rangeMap[y][x + 1] = mark;
      }
      if (y != 0 && MAP_STATE->rangeMap[y - 1][x] < 0) {
        MAP_STATE->rangeMap[y - 1][x] = mark;
      }
      if (y != MAP_STATE->unk_1c4->mapHeight - 1 &&
          MAP_STATE->rangeMap[y + 1][x] < 0) {
        MAP_STATE->rangeMap[y + 1][x] = mark;
      }
    }
  }
}

/* Counts the current player's units of the given type (slots 1-50) that
 * carry nothing in their first cargo slot. */
int Ai_CountEmptyTransports(int type) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type != UNIT_NONE && unit->type == type) {
      if (unit->cargo[0] == 0) {
        count++;
      }
    }
  }
  return count;
}

/* Counts the current player's Infantry and Mech (slots 1-50) with
 * transportWanted 2. */
int Ai_CountFootWantingTransport(void) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type != UNIT_NONE && unit->type <= UNIT_MECH) {
      if (unit->transportWanted == 2) {
        count++;
      }
    }
  }
  return count;
}

/* Counts the current player's units other than Infantry and Mech (slots
 * 1-50) with transportWanted 2, which the Lander and Black Boat builders
 * read as waiting for sea transport. */
int Ai_CountVehiclesWantingTransport(void) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type != UNIT_NONE && unit->type > UNIT_MECH) {
      if (unit->transportWanted == 2) {
        count++;
      }
    }
  }
  return count;
}

/* Counts the current player's ships that have a weapon (slots 1-50). */
int Ai_CountArmedShips(void) {
  int count = 0;
  int slot;
  MapUnit* unit;

  for (slot = Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer] + 51;
       slot++) {
    unit = &MAP_STATE->units[slot];
    if (unit->type != UNIT_NONE &&
        (Ov000_gUnitStats[unit->type].moveGroup & MOVE_GROUP_SEA)) {
      if (UnitType_GetAttackKind(unit->type) != ATTACK_KIND_NONE) {
        count++;
      }
    }
  }
  return count;
}

/* Tries each site AiSite_PickNearestFoot(3) offers until one gets a Black Boat or none
 * are left. The range map is filled with a Mech's moves (FAR_AWAY) from the
 * site, then widened by RangeMap_SpreadOverSea for each move point a Lander has. A site
 * is marked (kind 0xfe) and skipped when an enemy ground unit other than a foot unit can
 * reach it. Otherwise it counts the current player's units in reach: foot units
 * waiting for transport (plus the other foot units when no capturable tile or
 * own airport is in reach), against empty Black Boats and Landers. Returns 1
 * when it queued a build. The name is a guess from the unit type. */
BOOL AiTurn_TryBuildBlackBoatNearSite(void) {
  /* needed to match: this declaration order */
  MapUnit* unit;
  s8 otherWaiting;
  s8 emptyLanders;
  s8 footWaiting;
  AiSite* site;
  s8 footNotWaiting;
  s8 emptyBoats;
  int slot;
  u8 transports;
  BOOL captureTileNear;
  int i;
  int x;
  int y;
  int index;

  transports =
      Ai_CountUnitsOfType(UNIT_LANDER) + Ai_CountUnitsOfType(UNIT_BLACK_BOAT);
  for (;;) {
    index = AiSite_PickNearestFoot(3);
    if (index == -1) {
      AiSites_ClearAllMarks();
      return 0;
    }
    site = &data_0229e5bc[index];
    /* needed to match: x read through the table, y through site */
    RangeMap_FillMoves(data_0229e5bc[index].x, site->y, UNIT_MECH, FAR_AWAY, 0);
    for (i = 0;
         i < Player_GetUnitMoveRange(MAP_STATE->currentPlayer, UNIT_LANDER);
         i++) {
      RangeMap_SpreadOverSea(i + FAR_AWAY + 1);
    }
    otherWaiting = 0;
    footWaiting = 0;
    footNotWaiting = 0;
    emptyLanders = 0;
    emptyBoats = 0;
    captureTileNear = 0;
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        if (MAP_STATE->rangeMap[y][x] <= FAR_AWAY) {
          if (Map_IsCapturableByActingUnit(x, y) == 1 ||
              (MAP_TILE_TERRAIN(
                   MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]) ==
                   TERRAIN_AIRPORT &&
               MAP_STATE->unk_04 ==
                   (MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)] & 0xe0))) {
            captureTileNear = 1;
          }
        }
        slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
        if (slot == 0) {
          continue;
        }
        unit = &MAP_STATE->units[slot];
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[(slot >> 6) + 1].team) {
          if (unit->type <= UNIT_MECH) {
            continue;
          }
          if (!(Ov000_gUnitStats[unit->type].moveGroup &
                (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
            continue;
          }
          if (MAP_STATE->rangeMap[y][x] <= FAR_AWAY) {
            goto reject;
          }
          continue;
        }
        if (Unit_IsCurrentPlayers(slot) != 1) {
          continue;
        }
        if (MAP_STATE->rangeMap[y][x] <= FAR_AWAY) {
          if (unit->transportWanted == 2) {
            if (unit->type <= UNIT_MECH) {
              footWaiting++;
            } else {
              otherWaiting++;
            }
          } else if (unit->type <= UNIT_MECH) {
            footNotWaiting++;
          }
        }
        if (unit->type == UNIT_LANDER && unit->cargo[0] == 0) {
          emptyLanders++;
        }
        if (unit->type == UNIT_BLACK_BOAT && unit->cargo[0] == 0) {
          emptyBoats++;
        }
      }
    }
    if (!captureTileNear) {
      footWaiting += footNotWaiting;
    }
    emptyLanders -= otherWaiting >> 2;
    if (emptyLanders < 0) {
      emptyLanders = 0;
    }
    /* each empty Lander counts as one boat, less one per four other units waiting */
    if ((emptyBoats + 1 + emptyLanders) * 3 <= footWaiting) {
      gUnitAction.target = UNIT_BLACK_BOAT;
      AiTurn_QueueBuildAtSite(index);
      return 1;
    }
    if (footWaiting != 0 && transports == 0) {
      gUnitAction.target = UNIT_BLACK_BOAT;
      AiTurn_QueueBuildAtSite(index);
      AiSites_ClearAllMarks();
      return 1;
    }
  reject:
    site->kind = 0xfe;
  }
}

/* End-actions step 8, third sub-step. Tries to start building a Black Boat
 * to carry foot units, the way AiTurn_TryBuildLander does for Landers:
 * needs bit 2 of the profile byte that AiProfile_HasBuildFlag tests and the funds
 * for one. AiTurn_TryBuildBlackBoatNearSite runs first and ends the step when it returns 1.
 * AiSite_PickNearestFoot(3), then AiSite_PickFirst(3), pick the factory (-1: none).
 * With no foot unit waiting (Ai_CountFootWantingTransport) nothing is built. When the
 * player has a Lander or a Black Boat, the new one needs four waiting foot
 * units per empty Black Boat, itself included, and four more per empty
 * Lander left over after one per four waiting vehicles. The name is a
 * guess. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildBlackBoatTransport(void) {
  Unk_02080440_48* state;
  int spareLanders;
  int footWaiting;
  int emptyBoatsAfter;
  int index;
  int boats;
  int landers;

  if (AiProfile_HasBuildFlag(4) == 0) {
    return 0;
  }
  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_BLACK_BOAT) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  if (AiTurn_TryBuildBlackBoatNearSite() == 1) {
    return 1;
  }
  index = AiSite_PickNearestFoot(3);
  if (index == -1) {
    index = AiSite_PickFirst(3);
    if (index == -1) {
      return 0;
    }
  }
  landers = Ai_CountUnitsOfType(UNIT_LANDER);
  boats = Ai_CountUnitsOfType(UNIT_BLACK_BOAT);
  footWaiting = Ai_CountFootWantingTransport();
  if (footWaiting == 0) {
    return 0;
  }
  if (landers + boats != 0) {
    landers = Ai_CountEmptyTransports(UNIT_LANDER);
    emptyBoatsAfter = Ai_CountEmptyTransports(UNIT_BLACK_BOAT) + 1;
    spareLanders = landers - (Ai_CountVehiclesWantingTransport() >> 2);
    if (spareLanders < 0) {
      spareLanders = 0;
    }
    if ((emptyBoatsAfter + spareLanders) * 4 > footWaiting) {
      return 0;
    }
  }
  gUnitAction.target = UNIT_BLACK_BOAT;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* End-actions step 8, fourth sub-step. Tries to start building a Black
 * Boat: needs bit 2 of the profile byte that AiProfile_HasBuildFlag tests and the
 * funds for one. AiSite_PickForBuild(3) picks the factory (-1: none).
 * Ai_CountArmedShips counts the player's ships that have a weapon; the AI wants
 * one Black Boat per three of them, the new one included (a ship count of
 * 2 is enough for the first). The name is a guess from the unit type.
 * Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildBlackBoat(void) {
  Unk_02080440_48* state;
  int index;
  int boats;

  if (AiProfile_HasBuildFlag(4) == 0) {
    return 0;
  }
  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_BLACK_BOAT) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  index = AiSite_PickForBuild(3);
  if (index == -1) {
    return 0;
  }
  boats = Ai_CountUnitsOfType(UNIT_BLACK_BOAT);
  if (Ai_CountArmedShips() + 1 < (boats + 1) * 3) {
    return 0;
  }
  gUnitAction.target = UNIT_BLACK_BOAT;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}

/* Lander version of AiTurn_TryBuildBlackBoatNearSite. Tries each site
 * AiSite_PickForLander(3) offers until one gets a Lander or none are left. The
 * range map is filled with a Mech's moves (FAR_AWAY) from the site, then
 * widened by RangeMap_SpreadOverSea for each move point a Lander has. A site
 * is marked (kind 0xfe) and skipped when an enemy ground unit other than a
 * foot unit is anywhere in the filled range, or when the current player has
 * no unit in reach other than foot units (vehicles count when waiting for
 * transport or when they move on the ground). It then counts the current
 * player's units in reach that wait for transport; without a capturable tile
 * or own airport in reach the other foot units count too. Foot units count
 * only past four per empty Black Boat. A Lander is built when there are four
 * of them per empty Lander, the new one included. Returns 1 when it queued a
 * build. The name is a guess from the unit type. */
BOOL AiTurn_TryBuildLanderNearSite(void) {
  /* needed to match: this declaration order */
  s8 footNotWaiting;
  s8 otherNotWaiting;
  s8 emptyLanders;
  s8 emptyBoats;
  MapUnit* unit;
  s8 otherWaiting;
  s8 footWaiting;
  int x;
  int slot;
  int i;
  BOOL captureTileNear;
  AiSite* site;
  int index;
  int y;

  for (;;) {
    index = AiSite_PickForLander(3);
    if (index == -1) {
      AiSites_ClearAllMarks();
      return 0;
    }
    site = &data_0229e5bc[index];
    RangeMap_FillMoves(data_0229e5bc[index].x, site->y, UNIT_MECH, FAR_AWAY, 0);
    for (i = 0;
         i < Player_GetUnitMoveRange(MAP_STATE->currentPlayer, UNIT_LANDER);
         i++) {
      RangeMap_SpreadOverSea(i + FAR_AWAY + 1);
    }
    otherWaiting = 0;
    footWaiting = 0;
    footNotWaiting = 0;
    otherNotWaiting = 0;
    emptyLanders = 0;
    emptyBoats = 0;
    captureTileNear = 0;
    for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
        if (MAP_STATE->rangeMap[y][x] < 0) {
          continue;
        }
        if (MAP_STATE->rangeMap[y][x] <= FAR_AWAY) {
          if (Map_IsCapturableByActingUnit(x, y) == 1 ||
              (MAP_TILE_TERRAIN(
                   MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)]) ==
                   TERRAIN_AIRPORT &&
               MAP_STATE->unk_04 ==
                   (MAP_STATE->mapTiles->tiles[MAP_TILE_INDEX(x, y)] & 0xe0))) {
            captureTileNear = 1;
          }
        }
        slot = MAP_STATE->mapTiles->unitAt[MAP_TILE_INDEX(x, y)];
        if (slot == 0) {
          continue;
        }
        unit = &MAP_STATE->units[slot];
        if (MAP_STATE->players[MAP_STATE->currentPlayer].team !=
            MAP_STATE->players[(slot >> 6) + 1].team) {
          if (unit->type <= UNIT_MECH) {
            continue;
          }
          if (Ov000_gUnitStats[unit->type].moveGroup &
              (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
            goto reject;
          }
          continue;
        }
        if (Unit_IsCurrentPlayers(slot) != 1) {
          continue;
        }
        if (MAP_STATE->rangeMap[y][x] <= FAR_AWAY) {
          if (unit->transportWanted == 2) {
            if (unit->type <= UNIT_MECH) {
              footWaiting++;
            } else {
              otherWaiting++;
            }
          } else if (unit->type <= UNIT_MECH) {
            footNotWaiting++;
          } else if (Ov000_gUnitStats[unit->type].moveGroup &
                     (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) {
            otherNotWaiting++;
          }
        }
        if (unit->type == UNIT_LANDER && unit->cargo[0] == 0) {
          emptyLanders++;
        }
        if (unit->type == UNIT_BLACK_BOAT && unit->cargo[0] == 0) {
          emptyBoats++;
        }
      }
    }
    if (otherWaiting + otherNotWaiting != 0) {
      if (!captureTileNear) {
        footWaiting += footNotWaiting;
      }
      footWaiting -= emptyBoats * 4;
      if (footWaiting < 0) {
        footWaiting = 0;
      }
      if ((emptyLanders + 1) * 4 <= otherWaiting + footWaiting) {
        gUnitAction.target = UNIT_LANDER;
        AiTurn_QueueBuildAtSite(index);
        AiSites_ClearAllMarks();
        return 1;
      }
    }
  reject:
    site->kind = 0xfe;
  }
}

/* End-actions step 8, second sub-step. Tries to start building a Lander:
 * needs bit 3 of the profile byte that AiProfile_HasBuildFlag tests and the funds for
 * one. AiTurn_TryBuildLanderNearSite runs first and ends the step when it returns 1.
 * AiSite_PickForLander(3), then AiSite_PickFirst(3), pick the factory (-1: none).
 * Ai_CountVehiclesWantingTransport counts the player's vehicles (type above Mech) with
 * MapUnit.transportWanted 2, Ai_CountFootWantingTransport the foot units with it, and
 * Ai_CountEmptyTransports the player's units of a type with nothing in cargo[0].
 * With no vehicle waiting nothing is built. When Landers exist already, the
 * new one needs four waiting units per empty Lander, itself included,
 * counting foot units only past four per empty Black Boat. The name is a
 * guess from the unit type. Returns 1 when it queued an action. */
BOOL AiTurn_TryBuildLander(void) {
  Unk_02080440_48* state;
  int vehiclesWaiting;
  int footWaiting;
  int index;
  int emptyLandersAfter;
  int count;

  if (AiProfile_HasBuildFlag(8) == 0) {
    return 0;
  }
  state = STATE;
  if ((u32)(Unit_GetValuePerHp(state->currentPlayer, UNIT_LANDER) * 10) >
      state->players[state->currentPlayer].funds) {
    return 0;
  }
  if (AiTurn_TryBuildLanderNearSite() == 1) {
    return 1;
  }
  index = AiSite_PickForLander(3);
  if (index == -1) {
    index = AiSite_PickFirst(3);
    if (index == -1) {
      return 0;
    }
  }
  count = Ai_CountUnitsOfType(UNIT_LANDER);
  vehiclesWaiting = Ai_CountVehiclesWantingTransport();
  if (vehiclesWaiting == 0) {
    return 0;
  }
  if (count != 0) {
    emptyLandersAfter = Ai_CountEmptyTransports(UNIT_LANDER) + 1;
    count = Ai_CountEmptyTransports(UNIT_BLACK_BOAT);
    footWaiting = Ai_CountFootWantingTransport() - count * 4;
    if (footWaiting < 0) {
      footWaiting = 0;
    }
    if (emptyLandersAfter * 4 > vehiclesWaiting + footWaiting) {
      return 0;
    }
  }
  gUnitAction.target = UNIT_LANDER;
  AiTurn_QueueBuildAtSite(index);
  return 1;
}
