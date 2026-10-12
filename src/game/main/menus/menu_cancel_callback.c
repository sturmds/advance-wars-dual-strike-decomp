#include "types.h"
#include "game/menu_button.h"

/* No direct callers: two menu button templates (data_02158f54 and
 * data_02158f7c) hold its address at offset 0x18. */
void Menu_CancelCallback(void) { Menu_CancelSelected(); }
