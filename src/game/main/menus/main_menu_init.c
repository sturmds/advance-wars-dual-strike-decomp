#include "types.h"
#include "game/main_menu.h"

void MainMenu_SetUnk0f_4(int value);
void MainMenu_ClearCursors(void);

/* Resets the main menu state: clears the cursors and the byte at 0x11. */
void MainMenu_Init(void) {
  data_021599d0->unk_11 = 0;
  MainMenu_SetUnk0f_4(0);
  MainMenu_ClearCursors();
}
