#include "types.h"
#include "game/co_select_screen.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_0215a9c8;

void Co_UnpackPicture(int co, int firstTile);
BOOL Skills_AreDisabled(void);
void Sound_PlaySe(int id);
BOOL CoSelect_CanLeaveBackCoEmpty(void);
BOOL CoSelectScreen_IsCoInOtherCell(u8 co); /* another cell holds co */
void CoSelectScreen_RefreshCellSkills(u8 column, u8 row);
void CoSelectScreen_DrawSkillSlots(void);
void CoSelectScreen_AddGridButtons(void);

/* The CO of a grid cell, and its flags 6 bytes on. Needed to match: coGrid
 * read as rows of 2. */
#define CELL_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])
#define CELL_FLAGS(column, row) \
  (((u8(*)[2]) & data_0229c73c->coGrid[6])[(column) - 1][row])
#define CELL_LOCKED 1

/* onConfirm of the CO list buttons. An empty cursor cell is refused unless
 * it is in the second row and CoSelect_CanLeaveBackCoEmpty allows it. When another cell
 * holds the picked CO, that cell gets the cursor cell's old CO (refused if
 * that cell is locked). Then closes the list and goes back to the grid. */
void CoSelectScreen_ConfirmCoList(MenuButton* button) {
  u8* column = &data_0229c73c->coGrid[(data_0229c73c->cursorColumn - 1) * 2];
  int co = column[data_0229c73c->cursorRow];
  int col;
  int row;

  if (co == 0 &&
      (data_0229c73c->cursorRow == 0 || CoSelect_CanLeaveBackCoEmpty() == 0)) {
    Sound_PlaySe(0x10009);
    return;
  }
  if (CoSelectScreen_IsCoInOtherCell(co) == 1) {
    for (col = 1; col < data_0229c73c->columnCount + 1; col++) {
      for (row = 0; row < data_0229c73c->rowCount; row++) {
        if (col == data_0229c73c->cursorColumn &&
            row == data_0229c73c->cursorRow) {
          continue;
        }
        if (co != CELL_CO(col, row)) {
          continue;
        }
        if (CELL_FLAGS(col, row) & CELL_LOCKED) {
          Sound_PlaySe(0x10009);
          return;
        }
        CELL_CO(col, row) = data_0229c73c->coBeforeList;
        /* each cell's picture takes 12 tiles from tile 0x2c on */
        Co_UnpackPicture(CELL_CO(col, row), ((col - 1) * 2 + row) * 12 + 0x2c);
        CoSelectScreen_RefreshCellSkills(col, row);
        CoSelectScreen_DrawSkillSlots();
        goto close;
      }
    }
  }
close:
  Sound_PlaySe(0x10006);
  MenuButtons_RemoveSelectable();
  CoSelectScreen_AddGridButtons();
  data_0229c73c->coListOpen = 0;
  if (Skills_AreDisabled() == 0) {
    MenuButton_Add(0, 0, &data_0215a9c8, 0xfd);
  } else {
    MenuButton_Add(0, 32, &data_0215a9c8, 0xfd);
  }
}
