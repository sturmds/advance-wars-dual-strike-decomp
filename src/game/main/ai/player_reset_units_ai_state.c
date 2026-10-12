#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void MapUnit_ResetAiState(MapUnit* unit);

/* Resets the AI fields of slots 1 to 50 of the given player's units. */
void Player_ResetUnitsAiState(int player) {
  int slot;

  for (slot = Ov000_gPlayerFirstUnitSlot[player] + 1;
       slot < Ov000_gPlayerFirstUnitSlot[player] + 51; slot++) {
    MapUnit* unit = &data_027c027c->unk_48[data_027c0284]->units[slot];

    if (unit->type != UNIT_NONE) {
      MapUnit_ResetAiState(unit);
    }
  }
}
