#include "types.h"
#include "game/co_select_screen.h"
#include "game/keypad.h"
#include "game/menu_button.h"

int CoSelectScreen_GetListCo(u8 slot);
void Co_UnpackPicture(int co, int firstTile);
void Sound_PlaySe(int id);
void CoSelectScreen_RefreshCellSkills(u8 column, u8 row);
void CoSelectScreen_DrawSkillSlots(void);

/* onSelect of a CO list button: puts the slot's CO into the cursor cell,
 * loads its picture (12 tiles per cell from tile 0x2c) and redraws. Keeps
 * the slot as listAnchor unless the move was sideways. */
void CoSelectScreen_SelectCoListSlot(MenuButton* button) {
  Sound_PlaySe(0x10002);
  data_0229c73c->unk_30 = button->id;
  CO_SELECT_CURSOR_CO = CoSelectScreen_GetListCo(data_0229c73c->unk_30);
  if (CO_SELECT_CURSOR_CO != 0) {
    Co_UnpackPicture(
        CO_SELECT_CURSOR_CO,
        (((data_0229c73c->cursorColumn - 1) << 1) + data_0229c73c->cursorRow) *
                12 +
            0x2c);
  }
  CoSelectScreen_RefreshCellSkills(data_0229c73c->cursorColumn,
                                   data_0229c73c->cursorRow);
  if (!(gKeypad->repeat & (PAD_KEY_LEFT | PAD_KEY_RIGHT)) ||
      (gKeypad->repeat & (PAD_KEY_UP | PAD_KEY_DOWN))) {
    data_0229c73c->listAnchor = data_0229c73c->unk_30;
  }
  CoSelectScreen_DrawSkillSlots();
}
