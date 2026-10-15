#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map.h"
#include "game/map_unit.h"

void Units_ClearAll(void);
/* needed to match: declared with s16 parameters, so the call narrows its
 * arguments as the original does (unit_spawn.c takes int). */
MapUnit* Unit_Spawn(s16 x, s16 y, s16 type);

/* DesignMapRecord.units: unit type in bits 0-5, owner - 1 in bits 6-7. */
#define DESIGN_UNIT_TYPE(code) ((code) & 63)
#define DESIGN_UNIT_OWNER(code) (((code) >> 6) + 1)

/* Replaces every unit on the map with the units of a player-made map record.
 * Each unit is spawned as its owner's (currentPlayer and currentUnitBase
 * are switched for the call); the current player is put back at the end. */
void DesignMap_SpawnUnits(DesignMapRecord* record) {
  int savedPlayer = MAP_STATE->currentPlayer;
  int savedUnitBase = MAP_STATE->currentUnitBase;
  int cell;
  int x;
  int y;

  Units_ClearAll();
  cell = 0;
  for (y = 0; y < MAP_STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < MAP_STATE->unk_1c4->mapWidth; x++) {
      if (record->units[cell] != 0) {
        MAP_STATE->currentPlayer = DESIGN_UNIT_OWNER(record->units[cell]);
        MAP_STATE->currentUnitBase =
            Ov000_gPlayerFirstUnitSlot[MAP_STATE->currentPlayer];
        Unit_Spawn(x, y, DESIGN_UNIT_TYPE(record->units[cell]));
      }
      cell++;
    }
  }
  MAP_STATE->currentPlayer = savedPlayer;
  MAP_STATE->currentUnitBase = savedUnitBase;
}
