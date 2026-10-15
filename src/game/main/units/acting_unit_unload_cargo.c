#include "types.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"

extern int
    data_022a6dd4; /* here: direction (0-3) of the drop tile; other code uses it as a list count */
extern s16 data_021684e8[]; /* dx, dy per direction */

#define MAP_STATE (data_027c027c->unk_48[data_027c0284])

/* Drops the unit in cargo slot cargoSlot of the acting unit on the tile
 * next to the acting unit's destination, in direction data_022a6dd4. A
 * unit left in slot 1 moves up to slot 0; with no cargo left the acting
 * unit loses MAP_UNIT_CARRYING. Human players' unloads count in
 * gPlayRecords. */
void ActingUnit_UnloadCargo(int cargoSlot) {
  MapUnit* cargo;

  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.unitsUnloaded < 999999) {
      gPlayRecords.unitsUnloaded++;
    }
  }
  cargo = &MAP_STATE->units[MAP_STATE->actingUnit->cargo[cargoSlot]];
  cargo->flags &= ~MAP_UNIT_CARRIED;
  cargo->flags &= ~MAP_UNIT_SELECTED;
  cargo->x = MAP_STATE->actingUnitDestX + data_021684e8[data_022a6dd4 * 2];
  cargo->y = MAP_STATE->actingUnitDestY + data_021684e8[data_022a6dd4 * 2 + 1];
  MAP_STATE->actingUnit->cargo[cargoSlot] = 0;
  if (cargoSlot == 0 && MAP_STATE->actingUnit->cargo[1] != 0) {
    MAP_STATE->actingUnit->cargo[0] = MAP_STATE->actingUnit->cargo[1];
    MAP_STATE->actingUnit->cargo[1] = 0;
  }
  if (MAP_STATE->actingUnit->cargo[0] == 0) {
    MAP_STATE->actingUnit->flags &= ~MAP_UNIT_CARRYING;
  }
}
