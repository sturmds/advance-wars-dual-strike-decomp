#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int MapScript_RunList10(int id, MapUnit* unit);
void func_020edb84(MapUnit* unit);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Display step of unit action 20 (see AiShowAction_Step2): tries
 * MapScript_RunList10 with 47 and the acting unit, and falls back to
 * func_020edb84 when that returns 0. */
void func_020b8230(void) {
  if (MapScript_RunList10(47, STATE->actingUnit)) {
    return;
  }
  func_020edb84(STATE->actingUnit);
}
