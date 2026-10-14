#include "types.h"
#include "game/menu_button.h"
#include "game/ov005_task.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba8.h"
#include "game/task.h"

void Ov005_FreeCoScreenWork(void);
void Ov005_InitCoScreen(void);
void Ov005_MoveCoGridCursor(void);
void Ov005_MoveCoPaletteCursor(void);
void Ov005_UpdateCoScreen(void);
void func_ov005_02362618(void);
void Ov005_OnCoGridButtonConfirm(void);
void Ov005_OnCoPaletteButtonConfirm(void);
void Ov005_OnCoGridButtonSelect(void);
void Ov005_SetCoVariantFromButton(void);

static MenuButtonTemplate data_ov005_0236abac[1] = {
#include "data/ov005/data_ov005_0236abac.inc.c"
};
TaskScriptCmd data_ov005_0236abfc[5] = {
#include "data/ov005/data_ov005_0236abfc.inc.c"
};
MenuButtonTemplate data_ov005_0236abd4[1] = {
#include "data/ov005/data_ov005_0236abd4.inc.c"
};
u8 data_ov005_0236ac24[0x6c] = {
#include "data/ov005/data_ov005_0236ac24.inc.c"
};
Ov005CoGridEntry data_ov005_0236ac90[OV005_CO_GRID_ENTRIES] = {
#include "data/ov005/data_ov005_0236ac90.inc.c"
};

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
