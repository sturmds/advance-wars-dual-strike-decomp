#include "types.h"
#include "game/Unk_0201d320.h"
#include "game/screen_other_map.h"

extern Unk_0201d320 data_022977f4[];

/* Event script command: switches to the second screen's state (data_027c0288
 * unk_04) and stores the record's unk_08 in the map state's unk_37 (3 after a
 * rules reset; meaning unknown), then switches back and goes on. */
BOOL EventCmd_SetSecondScreenMapUnk37(s16 index) {
  Unk_027c0280* savedGfx = data_027c0280;
  Unk_02080440* saved;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c027c->unk_48[data_027c0284]->unk_1c4->unk_37 =
      data_022977f4[index].unk_00->unk_08;
  Screen_SetCurrent(saved);
  Screen_SetCurrentGfx(savedGfx);
  data_022977f4[index].unk_00++;
  return TRUE;
}
