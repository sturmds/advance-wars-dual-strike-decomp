#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/neighbor_dir.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern int data_022a6dd4;
extern s16 data_021663dc
    []; /* 3 per side of the unit: bit for data_022a6dd4, tile x offset, tile y offset */

BOOL Tile_HasUnitToSupply(s16 x, s16 y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sets in data_022a6dd4 the bit of each of the four tiles around the acting
 * unit's destination that hold a unit to supply, and clears unk_1e of the
 * script slot. */
void Supply_FlagAdjacentTargets(ScriptSlot* slot) {
  u16 side;

  data_022a6dd4 = 0;
  for (side = 0; side < NEIGHBOR_DIR_COUNT; side++) {
    data_022a6dd4 |= data_021663dc[side * 3] *
                     Tile_HasUnitToSupply(
                         STATE->actingUnitDestX + data_021663dc[side * 3 + 1],
                         STATE->actingUnitDestY + data_021663dc[side * 3 + 2]);
  }
  slot->unk_1e = 0;
}
