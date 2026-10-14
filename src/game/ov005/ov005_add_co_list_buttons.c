#include "types.h"
#include "game/ov005_task.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/unlocks.h"
#include "game/Unk_ov005_0236bba0.h"
#include "game/text_print.h"

BOOL Co_IsAvailable(int coId);
u32 Ov005_GetCoListEntryName(int entry);

extern u8 data_ov005_0236a48c[];
extern u8 data_ov005_0236a5a0
    [][4]; /* per entry: x and y in pixels, then grid column and row */
void Ov005_MoveCoListCursor(void);
void func_ov005_0235d714(void);
void func_ov005_0235d13c(void);
void Ov005_SelectCoListEntry(void);

static MenuButtonTemplate data_ov005_0236a4d8 = {
#include "data/ov005/data_ov005_0236a4d8.inc.c"
};
MenuButtonTemplate data_ov005_0236a500 = {
#include "data/ov005/data_ov005_0236a500.inc.c"
};

/* Marks which of the 33 list entries are available, then gives each
 * available entry a button (id 128 + entry) and prints its name on the top
 * screen. An entry whose CO byte is 0 is always available and 255 never;
 * COs 28, 29 and 30 need game flags 64, 65 and 63; the others ask
 * Co_IsAvailable. Selects the button of work->selectedEntry. */
void Ov005_AddCoListButtons(Ov005Task* task) {
  Unk_ov005_0236bba0* work = task->work;
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;
  int i;
  u8* coIds = data_ov005_0236a48c;
  u8 active;
  u8 x;
  u8 y;
  u16* map;

  for (i = 0; i < 33; i++, coIds++) {
    if (*coIds == 0) {
      active = 1;
    } else if (*coIds == 30) {
      active = GameFlag_IsSet(63);
    } else if (*coIds == 28) {
      active = GameFlag_IsSet(64);
    } else if (*coIds == 29) {
      active = GameFlag_IsSet(65);
    } else if (*coIds == 255) {
      active = 0;
    } else {
      active = Co_IsAvailable(*coIds);
    }
    work->entryActive[i] = active;
  }

  for (i = 0; i < 33; i++) {
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    if (work->entryActive[i] != 0) {
      work->buttonIds[i] = i + 128;
      x = data_ov005_0236a5a0[i][0];
      y = data_ov005_0236a5a0[i][1];
      MenuButton_Add(x, y, &data_ov005_0236a4d8, work->buttonIds[i]);
      map = data_027c0280->layer0Map;
      work->nameSlots[i] = Text_PrintId(
          x >> 3, y >> 3, map, Ov005_GetCoListEntryName(i), 0, i * 16 + 65);
      Screen_SetCurrent(saved027c);
      Screen_SetCurrentGfx(saved0280);
    }
  }
  MenuButton_Select(work->buttonIds[work->selectedEntry]);
}
