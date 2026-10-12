#include "types.h"
#include "game/menu_button.h"

void Sound_PlaySe(u32 soundId);
void func_0203579c(void);

/* onConfirm of the win quote screen's button (data_02159d28); also called
 * with NULL when A, B or START is pressed. func_0203579c starts task
 * data_02159d50. */
void WinQuote_Close(MenuButton* button) {
  Sound_PlaySe(0x10006);
  MenuButtons_CloseAll();
  func_0203579c();
}
