#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

/* Script command: sets the flag at unk_40 (the flag UnitList_Close also sets
 * when its menu closes) and moves on to the next command, running it this
 * frame. */
BOOL ScriptSlot_CmdSetFlag(int slot) {
  *data_027c027c->unk_40 = 1;
  data_027c027c->scriptSlots[slot].pc++;
  return TRUE;
}
