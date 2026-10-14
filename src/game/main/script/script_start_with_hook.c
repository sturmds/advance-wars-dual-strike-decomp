#include "types.h"
#include "game/script_slot.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern ScriptCommand gMapOverviewScript[];

#define HOOKS (data_027c027c->unk_50[data_027c0284])

void ScriptSlot_Start(const void* data, int arg);

/* Starts a script, then runs the screen config's unk_4c hook if its unk_48 is set. */
void Script_StartWithHook(void) {
  ScriptSlot_Start(gMapOverviewScript, 0);
  if (HOOKS->unk_48 == NULL) {
    return;
  }
  HOOKS->unk_4c();
}
