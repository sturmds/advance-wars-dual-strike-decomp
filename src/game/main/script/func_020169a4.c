#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"

extern Unk_02080440* data_027c027c;

void Sound_StartSong(int id);

BOOL func_020169a4(int slot) {
  Sound_StartSong(data_027c027c->scriptSlots[slot].pc->arg);
  data_027c027c->scriptSlots[slot].pc++;
  return FALSE;
}
