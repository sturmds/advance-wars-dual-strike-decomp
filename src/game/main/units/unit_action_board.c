#include "types.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void ActingUnit_ResetCaptureIfMoved(void);
void Sound_PlaySe(int id);

#define GAME (data_027c027c->unk_48[data_027c0284])

/* Unit action 7: the acting unit boards the transport on its destination tile. */
void UnitAction_Board(void) {
  MapUnit* transport;

  if (GAME->players[GAME->currentPlayer].controller ==
      PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.unitsBoarded < 999999) {
      gPlayRecords.unitsBoarded++;
    }
  }
  transport =
      &GAME->units[GAME->mapTiles->unitAt
                       [GAME->actingUnitDestX +
                        GAME->mapTiles->rowStart[GAME->actingUnitDestY]]];
  ActingUnit_ResetCaptureIfMoved();
  if (transport->cargo[0] != 0) {
    transport->cargo[1] = GAME->actingUnitIndex;
  } else {
    transport->cargo[0] = GAME->actingUnitIndex;
  }
  GAME->actingUnit->flags |= MAP_UNIT_CARRIED;
  transport->flags |= MAP_UNIT_CARRYING;
  switch (GAME->actingUnit->type) {
    case UNIT_INFANTRY:
    case UNIT_MECH:
      Sound_PlaySe(148);
      break;
  }
}
