#include "types.h"
#include "game/menu_button.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bbac.h"

void Screen_SetCurrent(void* state);
void Screen_SetCurrentGfx(void* state);

extern MenuButtonTemplate data_ov005_0236b004;

extern Unk_02080440* data_027c027c;
extern Unk_027c0280* data_027c0280;
extern Unk_027c0288 data_027c0288;

void Ov005_SoundRoom_AddCellButtons(Unk_ov005_0236bbac* state) {
  Ov005CoButton* button;
  int i;
  int count;

  count = OV005_CO_CELL_COUNT - state->page * OV005_CELLS_PER_PAGE;
  if (count > OV005_CELLS_PER_PAGE) {
    count = OV005_CELLS_PER_PAGE;
  }
  state->buttonCount = count;
  for (i = 0; i < count; i++) {
    Unk_027c0280* saved0280;
    Unk_02080440* saved027c;

    button = &state->buttons[i];
    button->unk_00 = 1;
    button->id = OV005_CELL_BUTTON_ID + i;
    saved0280 = data_027c0280;
    Screen_SetCurrentGfx(data_027c0288.unk_00);
    saved027c = data_027c027c;
    Screen_SetCurrent(data_027c0288.unk_00);
    MenuButton_Add(button->x, button->y, &data_ov005_0236b004, button->id);
    Screen_SetCurrent(saved027c);
    Screen_SetCurrentGfx(saved0280);
  }
  if (state->selection >= count) {
    state->selection = count - 1;
  }
  MenuButton_Select(state->buttons[state->selection].id);
  state->aTouched = 0;
}
