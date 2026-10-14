#include "types.h"
#include "game/menu_button.h"
#include "game/screen_other_map.h"
#include "game/task.h"

extern const MenuButtonTemplate data_ov005_0236a500;
void Ov005_AllocCoListWork(void);
void Ov005_SetScrollSpeedMinusOnTouch(void);
void Ov005_SetUpCoListScreen(void);
void Ov005_UpdateCoListScreen(void);
void func_ov005_0235bad0(void);
void func_ov005_0235d6fc(void);

TaskScriptCmd data_ov005_0236a578[5] = {
#include "data/ov005/data_ov005_0236a578.inc.c"
};
MenuButtonTemplate data_ov005_0236a550 = {
#include "data/ov005/data_ov005_0236a550.inc.c"
};
static MenuButtonTemplate data_ov005_0236a528 = {
#include "data/ov005/data_ov005_0236a528.inc.c"
};
/* per entry: x and y in pixels, then grid column and row */
u8 data_ov005_0236a5a0[33][4] = {
#include "data/ov005/data_ov005_0236a5a0.inc.c"
};

void Ov005_AddStateScreenButtons(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  MenuButton_Add(0, 0, &data_ov005_0236a500, 241);
  MenuButton_Add(0, 0, &data_ov005_0236a528, 242);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
