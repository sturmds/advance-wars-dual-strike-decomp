#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/Unk_ov005_0236bbac.h"

void Screen_SetCurrent(void *state);
void Screen_SetCurrentGfx(void *state);
void MenuButton_Add(int x, int y, void *def, int id);
void MenuButton_Select(int id);

extern u8 data_ov005_0236b004[];

extern Unk_02080440 *data_027c027c;
extern Unk_027c0280 *data_027c0280;
extern Unk_027c0288 data_027c0288;

void func_ov005_02365010(Unk_ov005_0236bbac *state)
{
    int count = OV005_CO_CELL_COUNT - state->page * OV005_CELLS_PER_PAGE;
    int i;

    if (count > OV005_CELLS_PER_PAGE) {
        count = OV005_CELLS_PER_PAGE;
    }
    state->buttonCount = count;
    for (i = 0; i < count; i++) {
        Ov005CoButton *button = &state->buttons[i];
        Unk_027c0280 *saved0280;
        Unk_02080440 *saved027c;

        button->unk_00 = 1;
        button->id = OV005_CELL_BUTTON_ID + i;
        saved0280 = data_027c0280;
        Screen_SetCurrentGfx(data_027c0288.unk_00);
        saved027c = data_027c027c;
        Screen_SetCurrent(data_027c0288.unk_00);
        MenuButton_Add(button->x, button->y, data_ov005_0236b004, button->id);
        Screen_SetCurrent(saved027c);
        Screen_SetCurrentGfx(saved0280);
    }
    if (state->selection >= count) {
        state->selection = count - 1;
    }
    MenuButton_Select(state->buttons[state->selection].id);
    state->aTouched = 0;
}
