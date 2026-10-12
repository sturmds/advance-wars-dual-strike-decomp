#include "types.h"
#include "game/co_select_screen.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_0215a9c8;

void Co_UnpackPicture(int co, int firstTile);
BOOL Skills_AreDisabled(void);
void Sound_PlaySe(int id);
void CoSelectScreen_RefreshCellSkills(u8 column, u8 row);
void CoSelectScreen_AddGridButtons(void);

/* onCancel of a CO list button: puts back the CO the cursor cell had when
 * the list opened, closes the list and adds the grid's button 0xfd again
 * (moved down 32 pixels when skills are disabled). */
void CoSelectScreen_CancelCoList(MenuButton* button) {
  Sound_PlaySe(0x10009);
  MenuButtons_RemoveSelectable();
  CoSelectScreen_AddGridButtons();
  data_0229c73c->coListOpen = 0;
  CO_SELECT_CURSOR_CO = data_0229c73c->coBeforeList;
  if (CO_SELECT_CURSOR_CO != 0) {
    Co_UnpackPicture(
        CO_SELECT_CURSOR_CO,
        (((data_0229c73c->cursorColumn - 1) << 1) + data_0229c73c->cursorRow) *
                12 +
            0x2c);
  }
  CoSelectScreen_RefreshCellSkills(data_0229c73c->cursorColumn,
                                   data_0229c73c->cursorRow);
  if (Skills_AreDisabled() == 0) {
    MenuButton_Add(0, 0, &data_0215a9c8, 0xfd);
  } else {
    MenuButton_Add(0, 32, &data_0215a9c8, 0xfd);
  }
}
