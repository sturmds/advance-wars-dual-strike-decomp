#include "types.h"
#include "game/keypad.h"

void func_020d7c08(void);

#define BOOT_KEYS_FORBIDDEN (PAD_BUTTON_START | PAD_BUTTON_R)
#define BOOT_KEYS_REQUIRED (PAD_BUTTON_L | PAD_BUTTON_SELECT | PAD_KEY_RIGHT)

/* If the keys held at boot are L + Select + Right without Start or R, starts
 * the screen of func_020d7c08 (purpose unknown, it sets up both screens and
 * a task) and returns TRUE. main then skips the normal first task. */
BOOL Boot_TryStartKeyComboScreen(u16 keys) {
  if (keys & BOOT_KEYS_FORBIDDEN) {
    return FALSE;
  }
  if ((keys & BOOT_KEYS_REQUIRED) != BOOT_KEYS_REQUIRED) {
    return FALSE;
  }
  func_020d7c08();
  return TRUE;
}
