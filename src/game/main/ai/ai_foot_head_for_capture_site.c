#include "types.h"
#include "game/Unk_02080440.h"
#include "game/ai_unit_action.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillUnitMoves(MapUnit* unit);
int AiActingUnit_GetSearchMovement(void);
void RangeMap_FillMovesOverTurns(int x, int y, int unitType,
                                 int movement); /* fills rangeMap from (x, y) */
BOOL Map_IsSiloOrCapturableTile(int x, int y);
BOOL Terrain_IsHqBaseAirportPortOrTower(u8 tile);
void AiFoot_BoardTransportForSite(
    int distance);                /* called when the site is more than 2 away */
void AiFoot_BoardTransport(void); /* called when no site is in range */
void AiUnit_MoveTowardTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define MAP (STATE->mapTiles)
#define TILE_INDEX(x, y) ((x) + MAP->rowStart[y])
#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1)

#define NO_TILE 0x7fff

/* MapUnit.transportWanted: the transport a foot unit waits for.
 * AiEmptyTransport_GoToFootUnit sends APCs to 1, T-Copters to 1 to 3. */
#define WANTS_APC 1
#define WANTS_T_COPTER 2

/* Run twice by AiFoot_MoveAndAttack for the acting Infantry or Mech:
 * heads for a capturable property or silo in its range map.
 *
 * A unit above 60 hp first keeps its old target if it can still reach it,
 * no other unit claimed it and no unit stands there. Then it picks the
 * tile with the lowest score (2 * MapTiles.rangeScratch, minus 1 for an HQ,
 * base, airport, port or com tower), ties going to the higher rangeMap
 * value. On the first run tiles that another foot unit claimed
 * (Unk_02080440_6088.claimCount) are skipped; once AI_STEP_ATTACK_DONE is
 * set they are not. A unit above 60 hp claims its tile, and asks for an
 * APC when the tile is more than 2 away; with no tile left it asks for a
 * T-Copter. */

void AiFoot_HeadForCaptureSite(void) {
  int i;
  MapUnit* unit;
  MapUnit* acting;
  int targetX;
  int targetY;
  int bestX;
  int bestY;
  int bestRange;
  int bestScore;
  int range;
  int score;
  int x;
  int y;

  /* Clear unk_0c_4 when an own Lander or Black Boat is in move range. */
  if ((gAiTurnState->domainsOwn & AI_DOMAIN_SEA) &&
      STATE->actingUnit->unk_0c_4 == 1) {
    RangeMap_FillUnitMoves(STATE->actingUnit);
    for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
      unit = &STATE->units[i];
      /* needed to match: the UNIT_NONE test comes first */
      if (unit->type != UNIT_NONE &&
          (unit->type == UNIT_LANDER || unit->type == UNIT_BLACK_BOAT)) {
        if (STATE->rangeMap[unit->y][unit->x] >= 0) {
          STATE->actingUnit->unk_0c_4 = 0;
          break;
        }
      }
    }
  }
  acting = STATE->actingUnit;
  RangeMap_FillMovesOverTurns(acting->x, acting->y, acting->type,
                              AiActingUnit_GetSearchMovement());
  acting = STATE_IDX_FIRST->actingUnit;
  targetX = acting->aiTargetX;
  if (targetX != MAP_UNIT_NO_TARGET && acting->hp >= 61) {
    targetY = acting->aiTargetY;
    if (STATE->rangeMap[targetY][targetX] >= 0 &&
        Map_IsSiloOrCapturableTile(targetX, targetY)) {
      if (STATE->unk_6088[MAP->unk_1928[TILE_INDEX(targetX, targetY)]]
                  .claimCount == 0 &&
          MAP->unitAt[TILE_INDEX(targetX, targetY)] == 0) {
        STATE->unk_6088[MAP->unk_1928[TILE_INDEX(targetX, targetY)]]
            .claimCount++;
        if (MAP->rangeScratch[TILE_INDEX(targetX, targetY)] > 2) {
          AiFoot_BoardTransportForSite(
              MAP->rangeScratch[TILE_INDEX(targetX, targetY)]);
        }
        AiUnit_MoveTowardTile(targetX, targetY);
      }
    }
  }
  bestX = -1;
  bestY = -1;
  bestRange = NO_TILE;
  bestScore = bestRange;
  STATE->actingUnit->aiTargetX = MAP_UNIT_NO_TARGET;
  STATE->actingUnit->aiTargetY = MAP_UNIT_NO_TARGET;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] < 0) {
        continue;
      }
      if (!Map_IsSiloOrCapturableTile(x, y)) {
        continue;
      }
      if (STATE->unk_6088[MAP->unk_1928[TILE_INDEX(x, y)]].claimCount != 0 &&
          !(gAiTurnState->stepFlags & AI_STEP_ATTACK_DONE)) {
        continue;
      }
      range = STATE->rangeMap[y][x];
      score = MAP->rangeScratch[TILE_INDEX(x, y)] * 2;
      if (Terrain_IsHqBaseAirportPortOrTower(
              MAP_TILE_TERRAIN(MAP->tiles[TILE_INDEX(x, y)])) == TRUE) {
        score--;
      }
      if (score > bestScore) {
        continue;
      }
      if (score == bestScore && bestRange > range) {
        continue;
      }
      bestX = x;
      bestY = y;
      bestRange = range;
      bestScore = score;
    }
  }
  if (bestRange != NO_TILE) {
    if (STATE->actingUnit->hp >= 61) {
      STATE->unk_6088[MAP->unk_1928[TILE_INDEX(bestX, bestY)]].claimCount++;
    }
    STATE->actingUnit->aiTargetX = bestX;
    STATE->actingUnit->aiTargetY = bestY;
    if ((bestScore >> 1) > 2 && STATE->actingUnit->hp >= 61 &&
        !STATE->actingUnit->claimedLandmass) {
      AiFoot_BoardTransportForSite(bestScore >> 1);
      STATE->actingUnit->transportWanted = WANTS_APC;
    }
    AiUnit_MoveTowardTile(bestX, bestY);
  } else if (STATE->actingUnit->hp >= 61 &&
             !STATE->actingUnit->claimedLandmass) {
    AiFoot_BoardTransport();
    STATE->actingUnit->transportWanted = WANTS_T_COPTER;
  }
}
