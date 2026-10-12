#include "types.h"
#include "game/co_select_screen.h"
#include "game/menu_button.h"

void Sound_PlaySe(int id);
int CoSelectScreen_ListSlotOfCo(int co);
void CoSelectScreen_AddCoListButtons(void);

/* The CO of a grid cell, and its flags 6 bytes on. */
#define CELL_CO(column, row) \
  (((u8(*)[2])data_0229c73c->coGrid)[(column) - 1][row])
#define CELL_FLAGS(column, row) \
  (((u8(*)[2]) & data_0229c73c->coGrid[6])[(column) - 1][row])
#define CELL_LOCKED 1

/* Opens the CO list for the cursor cell, unless the cell is locked. The
 * list starts on the slot of the cell's CO, and the CO is kept so that
 * cancel can put it back. */
void CoSelectScreen_OpenCoList(void) {
  if (CELL_FLAGS(data_0229c73c->cursorColumn, data_0229c73c->cursorRow) &
      CELL_LOCKED) {
    Sound_PlaySe(0x10009);
    return;
  }
  Sound_PlaySe(0x10006);
  MenuButtons_RemoveSelectable();
  CoSelectScreen_AddCoListButtons();
  data_0229c73c->unk_30 = CoSelectScreen_ListSlotOfCo(
      CELL_CO(data_0229c73c->cursorColumn, data_0229c73c->cursorRow));
  data_0229c73c->coListOpen = 1;
  data_0229c73c->coBeforeList =
      CELL_CO(data_0229c73c->cursorColumn, data_0229c73c->cursorRow);
  data_0229c73c->listAnchor = data_0229c73c->unk_30;
  MenuButton_Close(0xfd);
}
