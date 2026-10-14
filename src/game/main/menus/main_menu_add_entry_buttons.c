#include "types.h"
#include "game/main_menu.h"
#include "game/menu_button.h"

MainMenuPage** data_0229c71c;
MainMenu data_0229c720;

void MainMenu_ShowHelpText(void);

extern MenuButtonTemplate data_02159aec;

/* Adds one button per visible entry of the current page, 24 pixels apart and
 * centred on y = 112, and selects the cursor entry (moving the cursor off a
 * hidden one). Then runs MainMenu_ShowHelpText. */
void MainMenu_AddEntryButtons(void) {
  MainMenuPage* page = *data_0229c71c;
  MainMenuEntry* entry = page->entries;
  int y =
      112 - ((page->entryCount + 1) * 3
             << 2); /* (count + 1) * 12; the shift form is needed to match */
  int i;

  MenuButtons_RemoveSelectable();
  if (data_021599d0->cursors[data_021599d0->page] >= page->entryCount) {
    data_021599d0->cursors[data_021599d0->page] = 0;
  }
  while (page->entries[data_021599d0->cursors[data_021599d0->page]].hidden) {
    data_021599d0->cursors[data_021599d0->page]++;
    if (page->entryCount == data_021599d0->cursors[data_021599d0->page]) {
      data_021599d0->cursors[data_021599d0->page] = 0;
    }
  }
  MenuButton_Select(data_021599d0->cursors[data_021599d0->page]);
  for (i = 0; i < page->entryCount; i++) {
    if (!entry->hidden) {
      MenuButton_Add(40, y, &data_02159aec, i);
    }
    y += 24;
    entry++;
  }
  MainMenu_ShowHelpText();
}
