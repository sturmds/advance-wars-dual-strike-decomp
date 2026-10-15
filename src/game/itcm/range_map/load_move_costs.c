#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/co_data.h"
#include "game/range_map.h"

extern MoveSearch* data_027c0758;

#define PLAYER (MAP_STATE->players[MAP_STATE->unk_1e])

/* A movement cost chart: one row of 32 terrain costs per move type, -1 where
 * the move type cannot enter. */
typedef const s8 MoveCostRow[TERRAIN_ID_COUNT];

/* Copies the move cost of each terrain id for a unit type into the move
 * search, from the movement chart of the CO that unk_1e points to. No CO
 * effects (coEffectsOn 0): CO 0's chart. */
void MoveSearch_LoadMoveCosts(int unitType) {
  UnitStats* stats = &Ov000_gUnitStats[unitType];
  int moveType = stats->moveType;
  int terrain;
  int coId;

  for (terrain = 0; terrain < TERRAIN_ID_COUNT; terrain++) {
    if (MAP_STATE->coEffectsOn == 0) {
      coId = 0;
    } else {
      coId = PLAYER.front.id & PLAYER_CO_ID_MASK;
    }
    /* Needed to match: index the chart as rows; a byte offset of
         * moveType * 32 compiles to other registers. */
    data_027c0758->moveCost[terrain] =
        ((MoveCostRow*)gCoTable[coId]
             .blocks[PLAYER.front.powerState]
             .movementCost[PLAYER.moveChart])[moveType][terrain];
  }
}
