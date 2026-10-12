#include "types.h"
#include "game/menu_button.h"

extern const MenuButtonTemplate data_02159d28;

/* Adds the button whose onConfirm is WinQuote_Close. */
MenuButton* WinQuote_AddCloseButton(void) {
  return MenuButton_Add(0, 0, &data_02159d28, 0);
}
