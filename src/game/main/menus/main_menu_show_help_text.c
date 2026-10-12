#include "types.h"
#include "game/main_menu.h"
#include "game/menu_button.h"

void HelpText_Show(u32 textId, int tile);
void MainMenu_ShowRecordHelp(int a);

/* Text group 0x8000 holds the help text of the main menu entries. */
#define HELP_TEXT(index) (0x80000000 + (index))

/* Shows the help text of the selected main menu entry on the second screen,
 * picked by the entry's kind and, for some kinds, by
 * the page. Kinds 31-33 draw something else with MainMenu_ShowRecordHelp. Page 0 has
 * no text for kinds 11 and 12: textId is then unset, as in the original. */
void MainMenu_ShowHelpText(void) {
  u32 textId;

  switch ((*data_0229c71c)->entries[Menu_GetSelectedId()].kind) {
    case 0:
      textId = HELP_TEXT(0);
      break;
    case 1:
      textId = HELP_TEXT(0x6);
      break;
    case 2:
      textId = HELP_TEXT(0x3);
      break;
    case 3:
      textId = HELP_TEXT(0x11);
      break;
    case 4:
      textId = HELP_TEXT(0x20);
      break;
    case 5:
      textId = HELP_TEXT(0x9);
      break;
    case 6:
      textId = HELP_TEXT(0x19);
      break;
    case 7:
      textId = HELP_TEXT(0xd);
      break;
    case 8:
      textId = HELP_TEXT(0x10);
      break;
    case 9:
      textId = HELP_TEXT(0x18);
      break;
    case 10:
      textId = HELP_TEXT(0xa);
      break;
    case 11:
      switch (data_021599d0->page) {
        case 1:
          textId = HELP_TEXT(0x2);
          break;
        case 2:
          textId = HELP_TEXT(0x8);
          break;
        case 3:
          textId = HELP_TEXT(0x5);
          break;
        case 4:
          textId = HELP_TEXT(0x15);
          break;
        case 5:
          textId = HELP_TEXT(0x21);
          break;
      }
      break;
    case 12:
      switch (data_021599d0->page) {
        case 1:
          textId = HELP_TEXT(0x1);
          break;
        case 2:
          textId = HELP_TEXT(0x7);
          break;
        case 3:
          textId = HELP_TEXT(0x4);
          break;
        case 4:
          textId = HELP_TEXT(0x16);
          break;
        case 5:
          textId = HELP_TEXT(0x22);
          break;
      }
      break;
    case 13:
      textId = HELP_TEXT(0x14);
      break;
    case 14:
      textId = HELP_TEXT(0x13);
      break;
    case 15:
      textId = HELP_TEXT(0x12);
      break;
    case 16:
      textId = HELP_TEXT(0xc);
      break;
    case 17:
      textId = HELP_TEXT(0xb);
      break;
    case 18:
      textId = HELP_TEXT(0x17);
      break;
    case 19:
      textId = HELP_TEXT(0x1a);
      break;
    case 20:
      textId = HELP_TEXT(0x1b);
      break;
    case 21:
      textId = HELP_TEXT(0x1c);
      break;
    case 22:
      textId = HELP_TEXT(0x1d);
      break;
    case 23:
      textId = HELP_TEXT(0x1e);
      break;
    case 24:
      textId = HELP_TEXT(0x1f);
      break;
    case 25:
      switch (data_021599d0->page) {
        case 11:
          textId = HELP_TEXT(0x0e);
          break;
        case 12:
          textId = HELP_TEXT(0x23);
          break;
      }
      break;
    case 26:
      switch (data_021599d0->page) {
        case 11:
          textId = HELP_TEXT(0x0f);
          break;
        case 12:
          textId = HELP_TEXT(0x24);
          break;
      }
      break;
    case 27:
      textId = HELP_TEXT(0x25);
      break;
    case 28:
      textId = HELP_TEXT(0x26);
      break;
    case 29:
      textId = HELP_TEXT(0x27);
      break;
    case 30:
      textId = HELP_TEXT(0x28);
      break;
    case 31:
      MainMenu_ShowRecordHelp(0);
      return;
    case 32:
      MainMenu_ShowRecordHelp(1);
      return;
    case 33:
      MainMenu_ShowRecordHelp(2);
      return;
    default:
      return;
  }
  HelpText_Show(textId, 0x3c0);
}
