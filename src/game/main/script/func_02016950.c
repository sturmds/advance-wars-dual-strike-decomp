#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

int Sound_PlaySe(int id);

BOOL func_02016950(int slot) {
  Sound_PlaySe(data_027c027c->scriptSlots[slot].pc->arg);
  data_027c027c->scriptSlots[slot].pc++;
  return FALSE;
}
