#include "types.h"
#include "game/Unk_02080440.h"
#include "game/script_slot.h"
#include "game/campaign_map_music.h"
#include "game/song.h"

BOOL func_020169a4(int slot) {
  Sound_StartSong(data_027c027c->scriptSlots[slot].pc->arg);
  data_027c027c->scriptSlots[slot].pc++;
  return FALSE;
}
