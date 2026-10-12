#include "types.h"
#include "game/popup_menu.h"

void* PopupMenu_ResetStack(void);
void Sound_PlaySe(u32 soundId);
PopupMenuTask* PopupMenu_Open(const MenuItem* items, int x, int y, int style,
                              u16 selected, int center);

/* Opens a pop-up menu with the cursor on its first item and plays the menu
 * opening sound. */
void PopupMenu_OpenWithSound(const MenuItem* items, int x, int y, int style,
                             int center) {
  PopupMenu_ResetStack();
  Sound_PlaySe(0x1000a);
  PopupMenu_Open(items, x, y, style, 0, center);
}
