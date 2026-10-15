#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_0201d320.h"

extern Unk_0201d320 data_022977f4[];

/* Event script command: sets who controls a player slot (record value
 * 0x0a, a PLAYER_CONTROLLER_* value) in both the state's slotControllers and
 * the player record. Record value 0x08 is the slot. */
BOOL EventCmd_SetPlayerController(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];
  u8 player = script->unk_00->unk_08;

  MAP_STATE->slotControllers[player] = script->unk_00->unk_0a;
  MAP_STATE->players[player].controller = script->unk_00->unk_0a;
  script->unk_00++;
  return 1;
}
