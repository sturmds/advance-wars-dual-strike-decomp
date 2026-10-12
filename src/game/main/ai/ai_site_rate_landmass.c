#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/terrain.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
void RangeMap_MarkAdjacentTiles(int value);
int Ai_CountUnitsOfType(int type);
BOOL AiProfile_WantsSeaTransport(void);
BOOL Map_IsCapturableByActingUnit(s16 x, s16 y);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);
BOOL Unit_IsEnemyTile(int unitIndex, s16 x, s16 y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])
#define UNIT_OWNER(slot) (((slot) >> 6) + 1)

/* RangeMap_FillMoves movement; guess: high enough to act as no limit.
 * Tiles next to the reach get ADJACENT. */
#define ANY_DISTANCE 110
#define ADJACENT 111

/* Result bit: the landmass has a port, or a shoal, and the current player
 * has a Lander or the AI profile wants sea transport (for a shoal, only
 * when the player started with a sea factory). */
#define LANDMASS_SEA_TRANSPORT 0x100

/* Rates the landmass a Tank could reach from site (x, y). Returns 0 when it
 * holds a pipe seam, an enemy base, an enemy ground unit other than
 * Infantry and Mech, or has an enemy Piperunner next to it. Otherwise
 * returns 1 when a unit on it stands on a tile the acting unit can capture,
 * else the current player's Infantry and Mech on it plus 1. Either value
 * can have LANDMASS_SEA_TRANSPORT set. */
int AiSite_RateLandmass(int x, int y) {
  int capturable;
  int footUnits;
  int wantsTransport;
  int tx;
  int ty;
  int range;
  MapTiles* map;
  int tile;
  int slot;
  u32 type;

  capturable = 0;
  footUnits = 0;
  wantsTransport = 0;
  RangeMap_FillMoves(x, y, UNIT_TANK, ANY_DISTANCE, 0);
  RangeMap_MarkAdjacentTiles(ADJACENT);
  for (ty = 0; ty < STATE->unk_1c4->mapHeight; ty++) {
    for (tx = 0; tx < STATE->unk_1c4->mapWidth; tx++) {
      range = STATE->rangeMap[ty][tx];
      if (range < 0) {
        continue;
      }
      map = STATE->mapTiles;
      tile = TILE(tx, ty);
      if (MAP_TILE_TERRAIN(map->tiles[tile]) == TERRAIN_PIPE_SEAM) {
        return 0;
      }
      if (range > ANY_DISTANCE) {
        slot = map->unitAt[tile];
        if (slot == 0) {
          continue;
        }
        if (STATE->players[STATE->currentPlayer].team ==
            STATE->players[UNIT_OWNER(slot)].team) {
          continue;
        }
        if (STATE->units[slot].type != UNIT_PIPERUNNER) {
          continue;
        }
        return 0;
      }
      /* needed to match: the original reads currentUnitBase with ldrsh */
      if (MAP_TILE_TERRAIN(map->tiles[tile]) == TERRAIN_BASE &&
          Unit_IsEnemyTile((s16)STATE->currentUnitBase, tx, ty) == TRUE) {
        return 0;
      }
      if (!wantsTransport) {
        if (MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(tx, ty)]) ==
            TERRAIN_PORT) {
          if (Ai_CountUnitsOfType(UNIT_LANDER) != 0) {
            wantsTransport = 1;
          } else if (AiProfile_WantsSeaTransport() == TRUE) {
            wantsTransport = 1;
          }
        }
        if (!wantsTransport &&
            MAP_TILE_TERRAIN(STATE->mapTiles->tiles[TILE(tx, ty)]) ==
                TERRAIN_SHOAL) {
          if (Ai_CountUnitsOfType(UNIT_LANDER) != 0) {
            wantsTransport = 1;
          } else if ((gAiTurnState->factoryDomainsOwn & AI_DOMAIN_SEA) &&
                     AiProfile_WantsSeaTransport() == TRUE) {
            wantsTransport = 1;
          }
        }
      }
      slot = STATE->mapTiles->unitAt[TILE(tx, ty)];
      if (slot == 0) {
        continue;
      }
      if (Map_IsCapturableByActingUnit(tx, ty) == TRUE) {
        capturable = 1;
      }
      if (STATE->players[STATE->currentPlayer].team !=
          STATE->players[UNIT_OWNER(slot)].team) {
        type = STATE->units[slot].type;
        if ((Ov000_gUnitStats[type].moveGroup &
             (MOVE_GROUP_FOOT | MOVE_GROUP_TIRES | MOVE_GROUP_TREAD)) &&
            type > UNIT_MECH) {
          return 0;
        }
      } else if (Unit_IsCurrentPlayers(slot) == TRUE &&
                 STATE->units[slot].type <= UNIT_MECH) {
        footUnits++;
      }
    }
  }
  if (capturable) {
    return wantsTransport ? LANDMASS_SEA_TRANSPORT | 1 : 1;
  }
  return wantsTransport ? (footUnits + 1) | LANDMASS_SEA_TRANSPORT
                        : footUnits + 1;
}
