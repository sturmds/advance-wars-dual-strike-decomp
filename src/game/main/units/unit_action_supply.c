#include "types.h"
#include "game/Unk_02080440.h"
#include "game/play_records.h"
#include "game/player.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern const ScriptCommand data_02166450[];

ScriptSlot* ScriptSlot_Start(const ScriptCommand* script, int arg);

void UnitAction_Supply(void) {
  if (data_027c027c->unk_48[data_027c0284]
          ->players[data_027c027c->unk_48[data_027c0284]->currentPlayer]
          .controller == PLAYER_CONTROLLER_HUMAN) {
    if (gPlayRecords.suppliesAndRepairs < 999999) {
      gPlayRecords.suppliesAndRepairs++;
    }
  }
  ScriptSlot_Start(data_02166450, 0)->unk_20 = 1;
}
