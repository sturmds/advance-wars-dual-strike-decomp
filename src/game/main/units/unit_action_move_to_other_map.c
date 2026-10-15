#include "types.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"

void Unit_MoveToOtherMap(MapUnit* unit); /* puts the unit on the other map */

void UnitAction_MoveToOtherMap(void) {
  if (data_027c027c->unk_48[data_027c0284]
          ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
          .controller == PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.unitsSentToOtherMap < 999999) {
      gPlayRecords.unitsSentToOtherMap++;
    }
  }
  Unit_MoveToOtherMap(data_027c027c->unk_48[data_027c0284]->actingUnit);
}
