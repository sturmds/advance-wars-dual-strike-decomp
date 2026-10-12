#include "types.h"
#include "game/ai_build_state.h"
#include "game/ai_site.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

extern u8 gUnitFitsLander[];

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Unit_IsCurrentPlayers(s16 unitSlot);

#define STATE (data_027c027c->unk_48[data_027c0284])
#define TILE(x, y) ((x) + STATE->mapTiles->rowStart[y])

#define FAR_AWAY 110

/* End-actions step 7. For each record that is not skipped, fills the range map
 * with an Infantry's moves (FAR_AWAY) from the record's tile, then visits every
 * reachable tile that holds one of the current player's units. An Infantry or
 * Mech lowers unk_09 to the range map value there if that is smaller; a unit
 * with gUnitFitsLander set that waits for a T-Copter (transportWanted 2) does the
 * same for unk_0a. */
void func_02092134(void) {
  AiSite* record = data_0229e5bc;
  MapUnit* unit;
  int slot;
  int i;
  int x;
  int y;

  for (i = 0; i < data_021619c0->recordCount; i++, record++) {
    if (record->kind == 0xfd) {
      continue;
    }
    RangeMap_FillMoves(record->x, record->y, UNIT_INFANTRY, FAR_AWAY, 0);
    for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
      for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
        if (STATE->rangeMap[y][x] < 0) {
          continue;
        }
        slot = STATE->mapTiles->unitAt[TILE(x, y)];
        if (slot == 0) {
          continue;
        }
        if (Unit_IsCurrentPlayers(slot) != 1) {
          continue;
        }
        unit = &STATE->units[slot];
        if (unit->type == UNIT_INFANTRY || unit->type == UNIT_MECH) {
          if (record->unk_09 > STATE->rangeMap[y][x]) {
            record->unk_09 = STATE->rangeMap[y][x];
          }
        }
        if (gUnitFitsLander[unit->type] == 0 || unit->transportWanted != 2) {
          continue;
        }
        if (record->unk_0a > STATE->rangeMap[y][x]) {
          record->unk_0a = STATE->rangeMap[y][x];
        }
      }
    }
  }
}
