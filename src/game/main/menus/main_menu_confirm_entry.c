#include "types.h"
#include "game/main_menu.h"
#include "game/menu_button.h"
#include "game/unlocks.h"

void Sound_PlaySe(int id);
void MainMenu_StartPageFlip(int page, int slide);
void func_0202d6c0(int mode);
void func_020b9890(void);
void func_0202e868(void);

extern int
    gNextGameMode; /* next game mode, index into the table GameMode_StartNext runs */

/* onConfirm of the main menu buttons. The entry's kind either
 * flips to another menu page or picks the next game mode; actions 11, 12,
 * 25 and 26 depend on the current page. Picking a mode closes the menu. */
void MainMenu_ConfirmEntry(MenuButton* button) {
  MainMenuPage* page = *data_0229c71c;

  Sound_PlaySe(0x10006);
  data_021599d0->cursors[data_021599d0->page] = button->id;
  switch (page->entries[button->id].kind) {
    case 0:
      MainMenu_StartPageFlip(1, 1);
      return;
    case 1:
      MainMenu_StartPageFlip(2, 1);
      return;
    case 2:
      MainMenu_StartPageFlip(3, 1);
      return;
    case 3:
      MainMenu_StartPageFlip(4, 1);
      return;
    case 4:
      MainMenu_StartPageFlip(5, 1);
      return;
    case 5:
      gNextGameMode = 21;
      break;
    case 6:
      gNextGameMode = 31;
      break;
    case 7:
      MainMenu_StartPageFlip(7, 1);
      return;
    case 8:
      gNextGameMode = 20;
      break;
    case 9:
      gNextGameMode = 19;
      break;
    case 10:
      MainMenu_StartPageFlip(8, 1);
      return;
    case 11:
      switch (data_021599d0->page) {
        case 1:
          if (GameFlag_IsSet(32)) {
            MainMenu_StartPageFlip(11, 1);
            return;
          }
          gNextGameMode = 0;
          break;
        case 2:
          gNextGameMode = 12;
          break;
        case 3:
          MainMenu_StartPageFlip(13, 1);
          return;
        case 4:
          MainMenu_StartPageFlip(9, 1);
          return;
        case 5:
          if (GameFlag_IsSet(41)) {
            MainMenu_StartPageFlip(12, 1);
            return;
          }
          gNextGameMode = 14;
          break;
      }
      break;
    case 12:
      switch (data_021599d0->page) {
        case 1:
          MainMenu_StartPageFlip(10, 1);
          return;
        case 2:
          gNextGameMode = 13;
          break;
        case 3:
          gNextGameMode = 7;
          break;
        case 4:
          gNextGameMode = 11;
          break;
        case 5:
          gNextGameMode = 17;
          break;
      }
      break;
    case 13:
      gNextGameMode = 8;
      break;
    case 14:
      gNextGameMode = 9;
      break;
    case 15:
      gNextGameMode = 10;
      break;
    case 16:
      gNextGameMode = 29;
      break;
    case 17:
      gNextGameMode = 28;
      break;
    case 18:
      gNextGameMode = 30;
      break;
    case 19:
      gNextGameMode = 22;
      break;
    case 20:
      gNextGameMode = 23;
      break;
    case 21:
      gNextGameMode = 24;
      break;
    case 22:
      gNextGameMode = 25;
      break;
    case 23:
      gNextGameMode = 26;
      break;
    case 24:
      gNextGameMode = 27;
      break;
    case 25:
      switch (data_021599d0->page) {
        case 11:
          gNextGameMode = 0;
          break;
        case 12:
          gNextGameMode = 14;
          break;
      }
      break;
    case 26:
      switch (data_021599d0->page) {
        case 11:
          gNextGameMode = 1;
          break;
        case 12:
          gNextGameMode = 15;
          break;
      }
      break;
    case 27:
      gNextGameMode = 16;
      break;
    case 28:
      gNextGameMode = 18;
      break;
    case 29:
      gNextGameMode = 5;
      break;
    case 30:
      gNextGameMode = 6;
      break;
    case 31:
      gNextGameMode = 2;
      break;
    case 32:
      gNextGameMode = 3;
      break;
    case 33:
      gNextGameMode = 4;
      break;
    default:
      return;
  }
  func_0202d6c0(gNextGameMode);
  MenuButtons_RemoveSelectable();
  MenuButtons_CloseAll();
  func_020b9890();
  func_0202e868();
}
