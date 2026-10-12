#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern u8 gUnitSupplyKind[]; /* per unit type: 1 ground, 2 air, 3 sea */
extern u8 gTerrainIsProperty
    []; /* per terrain id: 1 for properties (city, HQ, airport, port, base, ...) */
extern u8 gTerrainSupplyKind
    []; /* per terrain id: kind of unit it supplies, 1 ground (city, HQ, base), 2 air, 3 sea */
extern u8 gTerrainFactoryKind
    []; /* per terrain id: 1 base, 2 airport, 3 port, else 0 */

int GetUnitMovement(int player, int unitType);
int Unit_GetValuePerHp(int playerIndex, int unitType);
int Unit_IsTileOwnedByTeam(int unitIndex,
                           int tile); /* 1: tile owned by the unit's team */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])
#define TILE(x, y) (MAP->tiles[TILE_INDEX(x, y)])

/* MapUnit.aiOrder value for which every tile is accepted. */
#define AI_ORDER_ANY_TILE 7

/* Whether the AI's acting unit may end its move on (x, y), used by the
 * attack planners for the tile they attack from. Occupied tiles are
 * refused; empty tiles that are not properties are fine. On a property
 * its team does not own,
 * a unit other than Infantry and Mech keeps off when a friendly Infantry
 * or Mech that has not moved yet is within 3 tiles (it could capture),
 * and keeps off a port when a friendly Lander or Black Boat that has not
 * moved is within a Lander's move. On its own team's base, airport or
 * port it keeps off unless the property supplies its kind of unit and
 * bit 1 of gAiTurnState->flags is set, or it is a Lander with one of the
 * player's own bases within 6 tiles, or a Black Boat with one within 3.
 * A player's own port is also kept free while it can afford a
 * Battleship. Each search has its own variables and the ship search uses
 * ternaries: both needed to match. */

BOOL Ai_CanEndMoveAt(int x, int y) {
  /* needed to match: declaration order sets registers and stack slots */
  int shipMaxY;
  int shipMaxX;
  int boatMaxY;
  int slot;
  int boatMaxX;
  int boatY;
  int boatMinY;
  int maxY;
  int boatMinX;
  int shipY;
  int landerMinX;
  MapUnit* unit;
  int landerMaxY;
  int landerMaxX;
  int maxX;
  int cy;
  int cx;
  int landerY;
  int minX;
  int landerMinY;
  int shipMinX;
  int minY;
  int shipX;
  int distance;
  int boatX;
  int landerX;

  if (STATE->actingUnit->aiOrder == AI_ORDER_ANY_TILE) {
    return TRUE;
  }
  if (MAP->unitAt[TILE_INDEX(x, y)] != 0) {
    return FALSE;
  }
  if (gTerrainIsProperty[MAP_TILE_TERRAIN(TILE(x, y))] == 0) {
    return TRUE;
  }
  /* needed to match: the s16 cast gives ldrsh */
  if (Unit_IsTileOwnedByTeam((s16)STATE->currentUnitBase, TILE(x, y)) == 0) {
    if (STATE->actingUnit->type <= UNIT_MECH) {
      return TRUE;
    }
    /* A friendly Infantry or Mech that has not moved may want to capture. */
    minX = x - 3;
    if (minX < 0) {
      minX = 0;
    }
    minY = y - 3;
    if (minY < 0) {
      minY = 0;
    }
    maxX = STATE->unk_1c4->mapWidth - 1;
    if (x + 3 < maxX) {
      maxX = x + 3;
    }
    maxY = STATE->unk_1c4->mapHeight - 1;
    if (y + 3 < maxY) {
      maxY = y + 3;
    }
    for (cy = minY; cy <= maxY; cy++) {
      for (cx = minX; cx <= maxX; cx++) {
        distance = 0;
        if (x - cx > 0) {
          distance += x - cx;
        } else {
          distance += cx - x;
        }
        if (y - cy > 0) {
          distance += y - cy;
        } else {
          distance += cy - y;
        }
        if (distance < 1 || distance > 3) {
          continue;
        }
        slot = MAP->unitAt[TILE_INDEX(cx, cy)];
        unit = &STATE->units[slot];
        if (unit->type == UNIT_NONE) {
          continue;
        }
        if (STATE->players[STATE->currentPlayer].team !=
            STATE->players[(slot >> 6) + 1].team) {
          continue;
        }
        if (unit->type > UNIT_MECH) {
          continue;
        }
        if (!(unit->flags & MAP_UNIT_WAITED)) {
          return FALSE;
        }
      }
    }
    /* A friendly Lander or Black Boat that has not moved may need this port. */
    if (MAP_TILE_TERRAIN(TILE(x, y)) == TERRAIN_PORT) {
      if (x - GetUnitMovement(STATE->currentPlayer, UNIT_LANDER) < 0) {
        shipMinX = 0;
      } else {
        shipMinX = x - GetUnitMovement(STATE->currentPlayer, UNIT_LANDER);
      }
      if (y - GetUnitMovement(STATE->currentPlayer, UNIT_LANDER) < 0) {
        shipY = 0;
      } else {
        shipY = y - GetUnitMovement(STATE->currentPlayer, UNIT_LANDER);
      }
      shipMaxX = x + GetUnitMovement(STATE->currentPlayer, UNIT_LANDER) <
                         STATE->unk_1c4->mapWidth - 1
                     ? x + GetUnitMovement(STATE->currentPlayer, UNIT_LANDER)
                     : STATE->unk_1c4->mapWidth - 1;
      shipMaxY = y + GetUnitMovement(STATE->currentPlayer, UNIT_LANDER) <
                         STATE->unk_1c4->mapHeight - 1
                     ? y + GetUnitMovement(STATE->currentPlayer, UNIT_LANDER)
                     : STATE->unk_1c4->mapHeight - 1;
      /* shipY starts at the lowest row: needed to match */
      for (; shipY <= shipMaxY; shipY++) {
        for (shipX = shipMinX; shipX <= shipMaxX; shipX++) {
          distance = 0;
          if (x - shipX > 0) {
            distance += x - shipX;
          } else {
            distance += shipX - x;
          }
          if (y - shipY > 0) {
            distance += y - shipY;
          } else {
            distance += shipY - y;
          }
          if (distance < 1 ||
              distance > GetUnitMovement(STATE->currentPlayer, UNIT_LANDER)) {
            continue;
          }
          slot = MAP->unitAt[TILE_INDEX(shipX, shipY)];
          unit = &STATE->units[slot];
          if (unit->type == UNIT_NONE) {
            continue;
          }
          if (STATE->players[STATE->currentPlayer].team !=
              STATE->players[(slot >> 6) + 1].team) {
            continue;
          }
          if (unit->type != UNIT_LANDER && unit->type != UNIT_BLACK_BOAT) {
            continue;
          }
          if (!(unit->flags & MAP_UNIT_WAITED)) {
            return FALSE;
          }
        }
      }
    }
    return TRUE;
  }

  if (gTerrainFactoryKind[MAP_TILE_TERRAIN(TILE(x, y))] == 0) {
    return TRUE;
  }
  if (gUnitSupplyKind[STATE->actingUnit->type] ==
          gTerrainSupplyKind[MAP_TILE_TERRAIN(TILE(x, y))] &&
      AI_TARGET_FLAGS->unk_1) {
    return TRUE;
  }
  /* Keep the player's own port free while it can afford a Battleship.
     * needed to match: the u32 cast makes the compare unsigned */
  if (MAP_TILE_TERRAIN(TILE(x, y)) == TERRAIN_PORT &&
      STATE->unk_04 == (TILE(x, y) & 0xe0)) {
    if ((u32)(Unit_GetValuePerHp(STATE->currentPlayer, UNIT_BATTLESHIP) * 10) <=
        STATE->players[STATE->currentPlayer].funds) {
      return FALSE;
    }
  }
  if (STATE->actingUnit->type == UNIT_LANDER) {
    landerMinX = x - 6;
    if (landerMinX < 0) {
      landerMinX = 0;
    }
    landerMinY = y - 6;
    if (landerMinY < 0) {
      landerMinY = 0;
    }
    landerMaxX = STATE->unk_1c4->mapWidth - 1;
    if (x + 6 < landerMaxX) {
      landerMaxX = x + 6;
    }
    landerMaxY = STATE->unk_1c4->mapHeight - 1;
    if (y + 6 < landerMaxY) {
      landerMaxY = y + 6;
    }
    for (landerY = landerMinY; landerY <= landerMaxY; landerY++) {
      for (landerX = landerMinX; landerX <= landerMaxX; landerX++) {
        distance = 0;
        if (x - landerX > 0) {
          distance += x - landerX;
        } else {
          distance += landerX - x;
        }
        if (y - landerY > 0) {
          distance += y - landerY;
        } else {
          distance += landerY - y;
        }
        if (distance < 1 || distance > 6) {
          continue;
        }
        if (MAP_TILE_TERRAIN(TILE(landerX, landerY)) == TERRAIN_BASE &&
            STATE->unk_04 == (TILE(landerX, landerY) & 0xe0)) {
          return TRUE;
        }
      }
    }
  }
  if (STATE->actingUnit->type == UNIT_BLACK_BOAT) {
    boatMinX = x - 3;
    if (boatMinX < 0) {
      boatMinX = 0;
    }
    boatMinY = y - 3;
    if (boatMinY < 0) {
      boatMinY = 0;
    }
    boatMaxX = STATE->unk_1c4->mapWidth - 1;
    if (x + 3 < boatMaxX) {
      boatMaxX = x + 3;
    }
    boatMaxY = STATE->unk_1c4->mapHeight - 1;
    if (y + 3 < boatMaxY) {
      boatMaxY = y + 3;
    }
    for (boatY = boatMinY; boatY <= boatMaxY; boatY++) {
      for (boatX = boatMinX; boatX <= boatMaxX; boatX++) {
        distance = 0;
        if (x - boatX > 0) {
          distance += x - boatX;
        } else {
          distance += boatX - x;
        }
        if (y - boatY > 0) {
          distance += y - boatY;
        } else {
          distance += boatY - y;
        }
        if (distance < 1 || distance > 3) {
          continue;
        }
        if (MAP_TILE_TERRAIN(TILE(boatX, boatY)) == TERRAIN_BASE &&
            STATE->unk_04 == (TILE(boatX, boatY) & 0xe0)) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}
