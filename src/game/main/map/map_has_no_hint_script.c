#include "types.h"
#include "game/map_header.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* TRUE when the current map has no "Hint" script (header +0x08).
 * No direct callers: probably a menu callback that greys out the option. */
s8 Map_HasNoHintScript(void) {
  return Ov000_gMapHeaders[data_027c027c->unk_48[data_027c0284]->unk_1c4->mapId]
             .hintScript == NULL;
}
