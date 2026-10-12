#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;
extern ScriptCommandFunc gScriptCommands[];

/* Counts down the slot's wait timer; once it reaches 0 and no callback is
 * pending, runs commands until a handler returns 0. */
void ScriptSlot_Run(int slot) {
  if (data_027c027c->scriptSlots[slot].start == NULL) {
    return;
  }
  if (data_027c027c->scriptSlots[slot].wait != 0) {
    data_027c027c->scriptSlots[slot].wait--;
    if (data_027c027c->scriptSlots[slot].wait != 0) {
      return;
    }
  }
  if (data_027c027c->scriptSlots[slot].callback != NULL) {
    return;
  }
  if (data_027c027c->scriptSlots[slot].start == NULL) {
    return;
  }
  if (data_027c027c->scriptSlots[slot].pc == NULL) {
    return;
  }
  while (gScriptCommands[data_027c027c->scriptSlots[slot].pc->opcode](slot)) {
  }
}
