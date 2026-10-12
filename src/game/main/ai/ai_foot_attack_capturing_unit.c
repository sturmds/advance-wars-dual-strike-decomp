#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/terrain.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
void RangeMap_MarkAdjacentTiles(int arg);
void AiUnit_PerformAction(s16 x, s16 y, int action, u8 arg3, u8 arg4);
BOOL Map_IsTileVisible(s16 x, s16 y);
void Ai_ClearActingUnitTarget(void);
BOOL FindAttackTile(int x, int y, s16* pos);
int Unit_CaptureEndsNextTurn(MapUnit* unit);
BOOL Map_IsTileOwnedByTeamOf(int player, int x,
                             int y); /* tile (x, y) belongs to player's team */

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

/* Infantry and mechs only: attacks the first visible enemy infantry or mech
 * whose capture of an HQ, base or airport (neutral or held by our team) would
 * finish next turn. */
void AiFoot_AttackCapturingUnit(void) {
  int x;
  int y;
  u8 tile;
  int terrain;
  MapUnit* unit;
  s16 pos[2];

  if (STATE->actingUnit->type > UNIT_MECH) {
    return;
  }
  RangeMap_FillUnitMoves(STATE->actingUnit);
  RangeMap_MarkAdjacentTiles(111);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      tile = STATE->mapTiles->tiles[TILE(x, y)];
      terrain = MAP_TILE_TERRAIN(tile);
      if (terrain != TERRAIN_HQ && terrain != TERRAIN_BASE &&
          terrain != TERRAIN_AIRPORT) {
        continue;
      }
      /* owner bits; needed to match: MAP_TILE_OWNER adds a mask */
      if ((tile >> 5) != 0 &&
          !Map_IsTileOwnedByTeamOf(STATE->currentPlayer, x, y)) {
        continue;
      }
      unit = &STATE->units[STATE->mapTiles->unitAt[TILE(x, y)]];
      if (unit->type == UNIT_NONE || unit->type > UNIT_MECH) {
        continue;
      }
      if (STATE->players[(STATE->actingUnitIndex >> 6) + 1].team ==
          STATE->players[MAP_UNIT_OWNER(STATE, unit)].team) {
        continue;
      }
      if (!Unit_CaptureEndsNextTurn(unit)) {
        continue;
      }
      if (!Map_IsTileVisible(x, y)) {
        continue;
      }
      if (!FindAttackTile(x, y, pos)) {
        continue;
      }
      Ai_ClearActingUnitTarget();
      AiUnit_PerformAction(pos[0], pos[1], AI_ACTION_ATTACK_UNIT,
                           STATE->mapTiles->unitAt[TILE(x, y)], 0);
      return;
    }
  }
}
