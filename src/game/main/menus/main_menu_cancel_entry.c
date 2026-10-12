#include "types.h"
#include "game/main_menu.h"
#include "game/menu_button.h"

void Sound_PlaySe(int id);
void MainMenu_StartPageFlip(int page, int slide);
void func_020b9864(void);
void func_0202e868(void);

extern int
    gNextGameMode; /* next game mode, index into the table GameMode_StartNext runs */

/* Back out of the main menu (likely onCancel of its buttons, beside
 * MainMenu_ConfirmEntry): remembers the cursor, then slides back to the
 * parent page. On pages 0 and 6 it picks game mode 32 and closes the
 * menu. */
void MainMenu_CancelEntry(MenuButton* button) {
  Sound_PlaySe(0x10009);
  data_021599d0->cursors[data_021599d0->page] = button->id;
  switch (data_021599d0->page) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      MainMenu_StartPageFlip(0, 0);
      return;
    case 7:
    case 8:
      MainMenu_StartPageFlip(6, 0);
      return;
    case 9:
      MainMenu_StartPageFlip(4, 0);
      return;
    case 10:
      MainMenu_StartPageFlip(1, 0);
      return;
    case 11: /* needed to match: not merged with case 10 */
      MainMenu_StartPageFlip(1, 0);
      return;
    case 12:
      MainMenu_StartPageFlip(5, 0);
      return;
    case 13:
      MainMenu_StartPageFlip(3, 0);
      return;
    case 0:
    case 6:
      break;
    default:
      return;
  }
  gNextGameMode = 32;
  MenuButtons_RemoveSelectable();
  MenuButtons_CloseAll();
  func_020b9864();
  func_0202e868();
}
