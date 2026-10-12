#include "types.h"
#include "game/menu_button.h"
#include "game/ov005_task.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba8.h"

extern const MenuButtonTemplate data_ov005_0236abac[];

void Ov005_AddCoGridButtons(Ov005Task* task) {
  Unk_ov005_0236bba8* work = task->work;
  int i;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  Ov005CoGridEntry* entry = work->grid;

  for (i = 0; i < OV005_CO_GRID_ENTRIES; i++, entry++) {
    if (entry->coId != -1 && entry->available != 0) {
      entry->buttonId = 0x80 + i;
      saved0280 = data_027c0280;
      Screen_SetCurrentGfx(data_027c0288.unk_00);
      saved027c = data_027c027c;
      Screen_SetCurrent(data_027c0288.unk_00);
      MenuButton_Add(entry->x - 8, entry->y - 8, data_ov005_0236abac,
                     entry->buttonId);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
  }
  MenuButton_Select(work->grid[work->cursor].buttonId);
}
