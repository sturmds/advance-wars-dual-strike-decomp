#include "types.h"
#include "game/popup_menu.h"

void* PopupMenu_ResetStack(void);
PopupMenuTask* PopupMenu_Create(const MenuItem* items, int x, int y, int style,
                                u16 selected, int center);

/* Empties the pop-up menu state stack, then creates a pop-up menu (see
 * PopupMenu_Create for the arguments). */
PopupMenuTask* PopupMenu_Open(const MenuItem* items, int x, int y, int style,
                              u16 selected, int center) {
  PopupMenu_ResetStack();
  return PopupMenu_Create(items, x, y, style, selected, center);
}
