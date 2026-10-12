#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void Units_ClearAll(void);
/* needed to match: declared with s16 parameters, so the call narrows its
 * arguments as the original does (unit_spawn.c takes int). */
MapUnit* Unit_Spawn(s16 x, s16 y, s16 type);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* DesignMapRecord.units: unit type in bits 0-5, owner - 1 in bits 6-7. */
#define DESIGN_UNIT_TYPE(code) ((code) & 63)
#define DESIGN_UNIT_OWNER(code) (((code) >> 6) + 1)

/* Replaces every unit on the map with the units of a player-made map record.
 * Each unit is spawned as its owner's (currentPlayer and currentUnitBase
 * are switched for the call); the current player is put back at the end. */
void DesignMap_SpawnUnits(DesignMapRecord* record) {
  int savedPlayer = STATE->currentPlayer;
  int savedUnitBase = STATE->currentUnitBase;
  int cell;
  int x;
  int y;

  Units_ClearAll();
  cell = 0;
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (record->units[cell] != 0) {
        STATE->currentPlayer = DESIGN_UNIT_OWNER(record->units[cell]);
        STATE->currentUnitBase =
            Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer];
        Unit_Spawn(x, y, DESIGN_UNIT_TYPE(record->units[cell]));
      }
      cell++;
    }
  }
  STATE->currentPlayer = savedPlayer;
  STATE->currentUnitBase = savedUnitBase;
}
