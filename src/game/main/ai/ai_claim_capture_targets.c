#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Map_IsSiloOrCapturableTile(int x, int y);
void RangeMap_FillMoves(int x, int y, int unitType, s16 movement, int arg4);
int Unit_GetUsableMovement(MapUnit* unit);
s16 AiUnit_GetSearchMovement(MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])
/* the Unk_02080440_6088 entry of the property on a tile */
#define PROPERTY_AT(x, y) \
  (STATE->unk_6088[STATE->mapTiles->unk_1928[TILE(x, y)]])

/* Queue routine run once by func_0208e7dc, the counterpart of
 * AiTurn_ReleaseCaptureClaims. For each of the AI player's infantry and
 * mechs with MapUnit.unk_0c_5 set (see AiUnit_MarkFinishingCapture), picks
 * the nearest tile it can reach that is a silo or capturable and that no
 * other unit has claimed, claims it (Unk_02080440_6088.claimCount) and stores
 * it as the unit's AI target. If the tile is within the distance
 * Unit_GetUsableMovement gives, the flag is cleared; if not, MapUnit.transportWanted is
 * set to 1 (meaning unknown). Ties go to the later tile. */
void AiTurn_ClaimCaptureTargets(void) {
  int i;
  MapUnit* unit;
  int bestX;
  int bestY;
  int best;
  int x;
  int y;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type <= UNIT_MECH && unit->unk_0c_5) {
      RangeMap_FillMoves(unit->x, unit->y, unit->type,
                         AiUnit_GetSearchMovement(unit), 0);
      bestY = -1;
      bestX = -1;
      best = 0x7fff;
      for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
        for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
          if (STATE->rangeMap[y][x] < 0) {
            continue;
          }
          if (!Map_IsSiloOrCapturableTile(x, y)) {
            continue;
          }
          if (PROPERTY_AT(x, y).claimCount != 0) {
            continue;
          }
          if (best >= STATE->rangeMap[y][x]) {
            bestX = x;
            bestY = y;
            best = STATE->rangeMap[y][x];
          }
        }
      }
      if (best != 0x7fff) {
        PROPERTY_AT(bestX, bestY).claimCount++;
        unit->aiTargetX = bestX;
        unit->aiTargetY = bestY;
        if (best <= Unit_GetUsableMovement(unit)) {
          unit->unk_0c_5 = 0;
        } else {
          unit->transportWanted = 1;
        }
      }
    }
  }
}
