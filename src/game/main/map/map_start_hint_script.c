#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"

void func_0201e954(void);
void* EventScript_Start(const void* script);

/* Starts the current map's "Hint" script (header +0x08). EventScript_Start
 * stores the script pointer in a free slot of a table, so it probably
 * starts a script; func_0201e954 runs first (meaning unknown). */
void Map_StartHintScript(void) {
  func_0201e954();
  EventScript_Start(
      Ov000_gMapHeaders[data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId]
          .hintScript);
}
