#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_0201d320 data_022977f4[];

/* Event script command: sets the front CO of player unk_08 (low byte) to the CO
 * id in the low 7 bits of unk_0a, then goes on. */
BOOL EventCmd_SetPlayerFrontCo(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];
  int player = (u8)script->unk_00->unk_08;

  data_027c027c->unk_48[data_027c0284]->players[player].front.id =
      script->unk_00->unk_0a & PLAYER_CO_ID_MASK;
  script->unk_00++;
  return TRUE;
}
