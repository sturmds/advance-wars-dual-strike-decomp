#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])
/* the Unk_02080440_6088 entry of the property on a tile */
#define PROPERTY_AT(x, y) \
  (STATE->unk_6088[STATE->mapTiles->unk_1928[TILE(x, y)]])

/* Queue routine run once at the end of the AI's unit actions (set by
 * func_0208e77c). For each of the AI player's infantry and mechs with
 * MapUnit.unk_0c_5 set (see AiUnit_MarkFinishingCapture), takes back the
 * claim it put on its capture target (Unk_02080440_6088.claimCount, raised
 * by AiFoot_PickCaptureTarget), or on the tile it stands on when it has no
 * target. Then clears the target and the flag. */
void AiTurn_ReleaseCaptureClaims(void) {
  int i;
  MapUnit* unit;

  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && unit->type <= UNIT_MECH && unit->unk_0c_5) {
      if (unit->aiTargetX != MAP_UNIT_NO_TARGET) {
        if (PROPERTY_AT(unit->aiTargetX, unit->aiTargetY).claimCount != 0) {
          PROPERTY_AT(unit->aiTargetX, unit->aiTargetY).claimCount--;
        }
      } else {
        if (PROPERTY_AT(unit->x, unit->y).claimCount != 0) {
          PROPERTY_AT(unit->x, unit->y).claimCount--;
        }
      }
      unit->aiTargetX = MAP_UNIT_NO_TARGET;
      unit->aiTargetY = MAP_UNIT_NO_TARGET;
      unit->unk_0c_5 = 0;
    }
  }
}
