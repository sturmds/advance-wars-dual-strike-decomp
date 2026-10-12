#include "types.h"
#include "game/event_script.h"
#include "game/Unk_02290118.h"

extern const EventScriptCommand data_021677f0[];

BOOL Player1Team_HasPlayerInGame(void);
int BattleSave_GetRecordId(int mode, int profile);

/* Starts the event script data_021677f0 when player 1's team has a player in
 * the game. Slot byte unk_10 gets BattleSave_GetRecordId(0, profile), where profile
 * is data_02290118.unk_00. */
void EventSequence_StartScript(void) {
  u8 profile = data_02290118.unk_00;
  EventScriptSlot* slot;

  if (!Player1Team_HasPlayerInGame()) {
    return;
  }
  slot = EventScript_Start(data_021677f0);
  slot->unk_10 = BattleSave_GetRecordId(0, profile);
}
