#include "types.h"
#include "game/Unk_022977a4.h"
#include "game/event_script.h"

extern EventScriptCommand data_02158570[];

/* Starts the short script data_02158570 that shows one text. The slot's
 * unk_14 gets the text id and data_022977a4.unk_02 the box kind (the callers
 * pass 21). Kinds below 33 are rebuilt from kind and kindHigh; the result is
 * the kind itself, as callers pass kindHigh 0. */
void EventScript_StartText(u32 textId, u32 kind, int kindHigh) {
  if (kind < 33) {
    kind = (kind | (kindHigh << 8)) & 0xff;
  }
  data_022977a4.unk_02 = kind;
  EventScript_Start(data_02158570)->unk_14 = (const EventScriptCommand*)textId;
}
