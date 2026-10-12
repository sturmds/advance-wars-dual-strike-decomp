#include "types.h"
#include "game/co_select_screen.h"
#include "game/menu_button.h"

void Sound_PlaySe(int id);
void CoSelectScreen_DrawSkillSlots(void);

/* Select handler of the grid buttons: moves the cursor to the cell whose
 * button was selected (id = (column - 1) * 2 + row) and redraws. */
void CoSelectScreen_OnGridButtonSelect(MenuButton* button) {
  Sound_PlaySe(0x10002);
  data_0229c73c->cursorColumn = (button->id >> 1) + 1;
  data_0229c73c->cursorRow = button->id & 1;
  CoSelectScreen_DrawSkillSlots();
}
