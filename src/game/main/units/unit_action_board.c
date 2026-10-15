#include "types.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"
#include "game/unit_stats.h"

void ActingUnit_ResetCaptureIfMoved(void);
void Sound_PlaySe(int id);

/* Unit action 7: the acting unit boards the transport on its destination tile. */
void UnitAction_Board(void) {
  MapUnit* transport;

  if (MAP_STATE->players[MAP_STATE->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.unitsBoarded < 999999) {
      gPlayRecords.unitsBoarded++;
    }
  }
  transport =
      &MAP_STATE
           ->units[MAP_STATE->mapTiles
                       ->unitAt[MAP_STATE->actingUnitDestX +
                                MAP_STATE->mapTiles
                                    ->rowStart[MAP_STATE->actingUnitDestY]]];
  ActingUnit_ResetCaptureIfMoved();
  if (transport->cargo[0] != 0) {
    transport->cargo[1] = MAP_STATE->actingUnitIndex;
  } else {
    transport->cargo[0] = MAP_STATE->actingUnitIndex;
  }
  MAP_STATE->actingUnit->flags |= MAP_UNIT_CARRIED;
  transport->flags |= MAP_UNIT_CARRYING;
  switch (MAP_STATE->actingUnit->type) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
      Sound_PlaySe(148);
      break;
  }
}
