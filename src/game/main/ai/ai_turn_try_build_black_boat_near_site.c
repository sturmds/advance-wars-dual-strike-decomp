#include "types.h"
#include "game/ai_site.h"
#include "game/map_unit.h"
#include "game/terrain.h"
#include "game/unit_action.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int Ai_CountUnitsOfType(int type);
int AiSite_PickNearestFoot(int kind);
void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
int Player_GetUnitMoveRange(int player, int unitType);
void RangeMap_SpreadOverSea(u8 value);
int Map_IsCapturableByActingUnit(s16 x, s16 y);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
void AiTurn_QueueBuildAtSite(int index);
void AiSites_ClearAllMarks(void);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define FAR_AWAY 110

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
    for (i = 0; i < Player_GetUnitMoveRange(STATE->currentPlayer, UNIT_LANDER);
         i++) {
      RangeMap_SpreadOverSea(i + FAR_AWAY + 1);
    }
    otherWaiting = 0;
    footWaiting = 0;
    footNotWaiting = 0;
    emptyLanders = 0;
    emptyBoats = 0;
    captureTileNear = 0;
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
        if (STATE->rangeMap[y][x] < 0) {
          continue;
        }
        if (STATE->rangeMap[y][x] <= FAR_AWAY) {
          if (Map_IsCapturableByActingUnit(x, y) == 1 ||
              (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(x, y)]) ==
                   TERRAIN_AIRPORT &&
               STATE->unk_04 == (STATE->mapTiles->tiles[TILE(x, y)] & 0xe0))) {
            captureTileNear = 1;
          }
        }
        slot = STATE->mapTiles->unitAt[TILE(x, y)];
        if (slot == 0) {
          continue;
        }
        unit = &STATE->units[slot];
        if (STATE->players[STATE->currentPlayer].team !=
            STATE->players[(slot >> 6) + 1].team) {
          if (unit->type <= UNIT_MECH) {
            continue;
          }
          if (!(Ov000_gUnitStats[unit->type].moveGroup &
                (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD))) {
            continue;
          }
          if (STATE->rangeMap[y][x] <= FAR_AWAY) {
            goto reject;
          }
          continue;
        }
        if (Unit_IsCurrentPlayers(slot) != 1) {
          continue;
        }
        if (STATE->rangeMap[y][x] <= FAR_AWAY) {
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
