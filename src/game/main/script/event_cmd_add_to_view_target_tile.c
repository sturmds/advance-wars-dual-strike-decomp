#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/Unk_02080440.h"

extern Unk_0201d320 data_022977f4[];

/* Event script command: adds the record's unk_08 and unk_0a to the game state's
 * tile (cursorX, cursorY).
 * Ends this frame's script run. */
BOOL EventCmd_AddToViewTargetTile(s16 index) {
  Unk_0201d320* script = &data_022977f4[index];

  data_027c027c->unk_48[data_027c0284]->cursorX += script->unk_00->unk_08;
  data_027c027c->unk_48[data_027c0284]->cursorY += script->unk_00->unk_0a;
  script->unk_00++;
  return FALSE;
}
